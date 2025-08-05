#pragma once

#include "../common.h"
#include "brex.h"
#include "brex_parser.h"
#include "brex_compiler.h"
#include "brex_executor.h"

namespace brex
{
    class NSRemapInfo
    {
    public:
        const std::string inns;
        const std::vector<std::pair<std::string, std::string>> nsmappings;
    };

    class REInfo
    {
    public:
        const std::string name;
        const std::u8string restr;
    };

    class RENSInfo
    {
    public:
        const NSRemapInfo nsinfo;
        const std::vector<REInfo> reinfos;
    };

    class ReNSRemapper
    {
    public:
        std::map<std::string, std::map<std::string, std::string>> nsmap;

        ReNSRemapper() {;}
        ~ReNSRemapper() {;}

        void addSingleNSMapping(const std::string& inns, const std::vector<std::pair<std::string, std::string>>& nsmapping)
        {
            std::for_each(nsmapping.begin(), nsmapping.end(), [this, &inns](const std::pair<std::string, std::string>& p) {
                this->nsmap[inns].insert(p);
            });
        }

        std::optional<std::string> resolveNamespace(const std::string& inns, const std::string& ns) const
        {
            auto nsiter = this->nsmap.find(inns);
            if(nsiter == this->nsmap.end()) {
                return std::nullopt;
            }

            auto nameiter = nsiter->second.find(ns);
            if(nameiter == nsiter->second.end()) {
                return std::make_optional(ns);
            }

            return std::make_optional(nameiter->second);
        }

        std::string remapName(const std::string& inns, const std::string& sname) const
        {
           auto biter = sname.find_last_of("::");

            if(biter == std::string::npos) {
                return inns + "::" + sname;
            }
            else {
                auto nsname = sname.substr(0, biter - 1);
                auto ename = sname.substr(biter + 1);

                auto resolvedns = this->resolveNamespace(inns, nsname);
                if(!resolvedns.has_value()) {
                    return sname;
                }
                else {
                    return resolvedns.value() + "::" + ename;
                }
            }
        }
    };

    class ReSystemEntry
    {
    public:
        const std::string ns;
        const std::string name;
        const std::string fullname;

        Regex* re;
        std::vector<std::string> deps;

        ReSystemEntry(const std::string& ns, const std::string& name, const std::string& fullname): ns(ns), name(name), fullname(fullname), re(nullptr) {;}
        virtual ~ReSystemEntry() {;}

        virtual bool isUnicode() const = 0;
        virtual std::optional<std::u8string> compileRegex() = 0;

        bool computeDeps(const ReNSRemapper& remapper)
        {
            std::set<std::string> constnames;
            std::set<std::string> envnames;
            bool envRequired = brex::RegexCompiler::gatherNamedRegexKeys(constnames, envnames, this->re);

            if(envRequired) {
                return false;
            }

            std::for_each(constnames.begin(), constnames.end(), [this, &remapper](const std::string& cname) {
                auto rname = remapper.remapName(this->ns, cname);

                if(std::find(this->deps.cbegin(), this->deps.cend(), rname) == this->deps.cend()) {
                    this->deps.push_back(rname);
                }
            });

            return true;
        }
    };

    class ReSystemUnicodeEntry : public ReSystemEntry
    {
    public:
        const std::u8string restr;
        UnicodeRegexExecutor* executor;

        ReSystemUnicodeEntry(const std::string& ns, const std::string& name, const std::string& fullname, const std::u8string& restr): ReSystemEntry(ns, name, fullname), restr(restr), executor(nullptr) {;}
        ~ReSystemUnicodeEntry() {;}

        bool isUnicode() const override { return true; }

        std::optional<std::u8string> compileRegex() override
        {
            auto pr = RegexParser::parseUnicodeRegex(this->restr, false);
            if(!pr.first.has_value() || !pr.second.empty()) {
                return !pr.second.empty() ? std::make_optional(pr.second[0].msg) : std::make_optional(u8"Invalid Regex");
            }

            this->re = pr.first.value();
            return std::nullopt;
        }
    };

    class ReSystemCEntry : public ReSystemEntry
    {
    public:
        const std::u8string restr;
        CRegexExecutor* executor;

        ReSystemCEntry(const std::string& ns, const std::string& name, const std::string& fullname, const std::u8string& restr): ReSystemEntry(ns, name, fullname), restr(restr), executor(nullptr) {;}
        ~ReSystemCEntry() {;}

        bool isUnicode() const override { return false; }

        std::optional<std::u8string> compileRegex() override
        {
            auto pr = RegexParser::parseCRegex(this->restr, false);
            if(!pr.first.has_value() || !pr.second.empty()) {
                return !pr.second.empty() ? std::make_optional(pr.second[0].msg) : std::make_optional(u8"Invalid Regex");
            }

            this->re = pr.first.value();
            return std::nullopt;
        }
    };

    class ReSystemResolverInfo
    {
    public:
        const std::string inns;
        const ReNSRemapper* remapper;

        ReSystemResolverInfo(const std::string& inns, const ReNSRemapper* remapper): inns(inns), remapper(remapper) {;}
        ~ReSystemResolverInfo() {;}
    };

    class ReSystem
    {
    public:
        ReNSRemapper remapper;
        std::vector<ReSystemEntry*> entries;
        std::map<std::string, std::vector<ReSystemEntry*>> depmap;

        ReSystem() {;}
        ~ReSystem() {;}

        void loadUnicodeEntry(const std::string& ns, const std::string& name, const std::string& fullname, const std::u8string& restr)
        {
            auto entry = new ReSystemUnicodeEntry(ns, name, fullname, restr);
            this->entries.push_back(entry);
        }

        void loadCStringEntry(const std::string& ns, const std::string& name, const std::string& fullname, const std::u8string& restr)
        {
            auto entry = new ReSystemCEntry(ns, name, fullname, restr);
            this->entries.push_back(entry);
        }

        void computeDependencies(std::vector<std::u8string>& errors)
        {
            std::for_each(this->entries.begin(), this->entries.end(), [this, &errors](ReSystemEntry* entry) {
                auto err = entry->compileRegex();
                if(err.has_value()) {
                    errors.push_back(err.value());
                }
                else {
                    auto depsok = entry->computeDeps(remapper);
                    
                    if(!depsok) {
                        errors.push_back(u8"Failed to compute dependencies for " + std::u8string(entry->fullname.cbegin(), entry->fullname.cend()));
                    }
                }
            });

            std::for_each(this->entries.begin(), this->entries.end(), [this, &errors](ReSystemEntry* entry) {
                if(this->depmap.find(entry->fullname) == this->depmap.end()) {
                    this->depmap.insert({ entry->fullname, {} });
                }

                std::for_each(entry->deps.begin(), entry->deps.end(), [this, entry, &errors](const std::string& dep) {
                    auto rdep = this->remapper.remapName(entry->ns, dep);
                    auto ii = std::find_if(this->entries.begin(), this->entries.end(), [rdep](ReSystemEntry* e) {
                        return e->fullname == rdep;
                    });

                    if(ii != this->entries.end()) {
                        this->depmap[entry->fullname].push_back(*ii);
                    }
                    else {
                        errors.push_back(u8"Failed to find dependency " + std::u8string(dep.cbegin(), dep.cend()) + u8" for " + std::u8string(entry->fullname.cbegin(), entry->fullname.cend()));
                    }
                });
            });
        }

        bool loadIntoNameMap(const std::string& fullname, const brex::Regex* pr, std::map<std::string, const brex::RegexOpt*>& nmap) 
        {
            if(pr->preanchor != nullptr || pr->postanchor != nullptr) {
                return false;
            }

            if(pr->re->tag != brex::RegexComponentTag::Single) {
                return false;
            }

            auto sre = static_cast<const brex::RegexSingleComponent*>(pr->re);
            if(sre->entry.isFrontCheck || sre->entry.isBackCheck || sre->entry.isNegated) {
                return false;
            }

            nmap.insert({ fullname, sre->entry.opt });

            return true;
        }

        static std::string resolveREName(const std::string& name, void* vremapper) {
            auto remapper = static_cast<ReSystemResolverInfo*>(vremapper);
            return remapper->remapper->remapName(remapper->inns, name);
        }

        bool processRERecursive(ReSystemEntry* entry, std::map<std::string, const RegexOpt*>& namedRegexes, std::vector<std::u8string>& errors, std::vector<std::string>& pending)
        {
            if(entry->re->ctag == RegexCharInfoTag::Unicode) {
                if(static_cast<ReSystemUnicodeEntry*>(entry)->executor != nullptr) {
                    return true;
                }
            }
            else {
                if(static_cast<ReSystemCEntry*>(entry)->executor != nullptr) {
                    return true;
                }
            }

            if(std::find(pending.begin(), pending.end(), entry->fullname) != pending.end()) {
                errors.push_back(u8"Cycle detected in regex with " + std::u8string(entry->fullname.cbegin(), entry->fullname.cend()));
                return false;
            }

            pending.push_back(entry->fullname);
 
            std::vector<ReSystemEntry*>& deps = this->depmap.find(entry->fullname)->second;
            auto recok = std::accumulate(deps.begin(), deps.end(), true, [this, &errors, &pending, &namedRegexes](bool acc, ReSystemEntry* dep) {
                auto ok = this->processRERecursive(dep, namedRegexes, errors, pending);
                if(!ok) {
                    return false;
                }
                
                auto okload = this->loadIntoNameMap(dep->fullname, dep->re, namedRegexes);
                return acc && okload;
            });

            pending.pop_back();
            if(!recok) {
                return false;
            }

            auto rmp = ReSystemResolverInfo(entry->ns, &this->remapper);
            std::vector<brex::RegexCompileError> compileerror;
            if(entry->re->ctag == RegexCharInfoTag::Unicode) {
                auto uentry = static_cast<ReSystemUnicodeEntry*>(entry);
                auto executor = RegexCompiler::compileUnicodeRegexToExecutor(uentry->re, namedRegexes, {}, false, &rmp, &ReSystem::resolveREName, compileerror);
                if(executor == nullptr) {
                    std::transform(compileerror.begin(), compileerror.end(), std::back_inserter(errors), [entry](const RegexCompileError& rce) {
                        return rce.msg + u8" in regex " + std::u8string(entry->fullname.cbegin(), entry->fullname.cend());
                    });
                    return false;
                }

                uentry->executor = executor;
            }
            else {
                auto centry = static_cast<ReSystemCEntry*>(entry);
                auto executor = RegexCompiler::compileCRegexToExecutor(centry->re, namedRegexes, {}, false, &rmp, &ReSystem::resolveREName, compileerror);
                if(executor == nullptr) {
                    std::transform(compileerror.begin(), compileerror.end(), std::back_inserter(errors), [entry](const RegexCompileError& rce) {
                        return rce.msg + u8" in regex " + std::u8string(entry->fullname.cbegin(), entry->fullname.cend());
                    });
                    return false;
                }

                centry->executor = executor;
            }

           
            return true;
        }

        static ReSystem processSystem(const std::vector<RENSInfo>& sinfo, std::vector<std::u8string>& errors)
        {
            ReSystem rsystem;

            //setup the remappings
            std::for_each(sinfo.cbegin(), sinfo.cend(), [&rsystem](const RENSInfo& nsi) {
                rsystem.remapper.addSingleNSMapping(nsi.nsinfo.inns, nsi.nsinfo.nsmappings);
            });

            //load the regex entries
            std::for_each(sinfo.cbegin(), sinfo.cend(), [&rsystem](const RENSInfo& nsi) {
                std::for_each(nsi.reinfos.cbegin(), nsi.reinfos.cend(), [&nsi, &rsystem](const REInfo& ri) {
                    if(ri.restr.ends_with('/')) {
                        rsystem.loadUnicodeEntry(nsi.nsinfo.inns, ri.name, nsi.nsinfo.inns + "::" + ri.name, ri.restr);
                    }
                    else {
                        rsystem.loadCStringEntry(nsi.nsinfo.inns, ri.name, nsi.nsinfo.inns + "::" + ri.name, ri.restr);
                    }
                });
            });

            //compute the dependencies
            rsystem.computeDependencies(errors);
            if(!errors.empty()) {
                return rsystem;
            }

            //compile the values
            std::map<std::string, const RegexOpt*> namedRegexes;
            for(auto iter = rsystem.entries.begin(); iter != rsystem.entries.end(); ++iter) {
                std::vector<std::string> pending;
                if(!rsystem.processRERecursive(*iter, namedRegexes, errors, pending)) {
                    return rsystem;
                }
            }

            return rsystem;
        }

        UnicodeRegexExecutor* getUnicodeRE(const std::string& fullname) const
        {
            auto iter = std::find_if(this->entries.begin(), this->entries.end(), [fullname](const ReSystemEntry* entry) {
                return entry->fullname == fullname;
            });

            if(iter == this->entries.end()) {
                return nullptr;
            }

            auto uentry = static_cast<ReSystemUnicodeEntry*>(*iter);
            return uentry->executor;
        }

        CRegexExecutor* getCStringRE(const std::string& fullname) const
        {
            auto iter = std::find_if(this->entries.begin(), this->entries.end(), [fullname](const ReSystemEntry* entry) {
                return entry->fullname == fullname;
            });

            if(iter == this->entries.end()) {
                return nullptr;
            }

            auto uentry = static_cast<ReSystemCEntry*>(*iter);
            return uentry->executor;
        }
    };
}
