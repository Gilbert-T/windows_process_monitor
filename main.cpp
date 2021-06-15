#include "globalConfig.h"
#include "GlobalOptions.h"
#include "ProcessInfo.h"
#include <prometheus/counter.h>
#include <prometheus/exposer.h>
#include <prometheus/registry.h>
#include "boost/filesystem.hpp"
#include <fstream>
#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <iostream>
#include <cstdlib>
#include<cstdio>
#include <windows.h> 
#include <tlhelp32.h> 
#include <tchar.h> 
#include <psapi.h>
#include <comdef.h>  // you will need this
#include <iomanip>
//  Forward declarations: 
using namespace prometheus;
using LabelType = std::pair<std::string, std::string>;
using Labels = std::vector<std::pair<std::string, std::string>>;

BOOL ListProcessModules(DWORD dwPID, std::vector<ProcessInfo>& vpi);
void printError(const TCHAR* msg);
void createMyGauges(Family<Gauge>& family, globalConfig* cfg, std::vector<std::pair<LabelType, Gauge*>>& vp);
void getProcessInfo(std::vector<ProcessInfo>& vpi);
bool isRunning(std::pair<std::string, std::string> label, const std::vector<ProcessInfo>& vpi);
int main(int argc, char** argv)
{       
    try {
        GlobalOptions* gop = GlobalOptions::instance(argc, argv);

        std::string filename = gop->configPath();
        if (boost::filesystem::exists(filename))
            std::cout << "found config file: " << filename << std::endl;
        else {
            std::cout << "can't find config file :" << filename << std::endl;
            return 1;
        }
            
        globalConfig* gcfg = globalConfig::instance(filename);

        std::string host = gop->listenAddress();
        // create an http server running on port 18080
        Exposer exposer{ host };

        // create a metrics registry with component=main labels applied to all its
        // metrics
        auto registry = std::make_shared<Registry>();

        // add a new counter family to the registry (families combine values with the
        // same name, but distinct label dimensions)
        auto& gauge_family = BuildGauge()
            .Name("windows_process_status")
            .Help("whether or not a process is running")
            .Labels({ {"status", "running"} })
            .Register(*registry);
        Labels labels{};
        std::vector<Gauge*> Gauges;

        std::vector<std::pair<LabelType, Gauge*>> vp;
        createMyGauges(gauge_family, gcfg, vp);
        // ask the exposer to scrape the registry on incoming scrapes
        exposer.RegisterCollectable(registry);
        std::vector<ProcessInfo> vpi;
        for (;;) {
            vpi.clear();
            getProcessInfo(vpi);
            //for every table pair
            std::cout << "Quering..." << std::endl;
            for (auto& i : vp) {
                //using Labels = std::vector<std::pair<std::string, std::string>>;                
                if (isRunning(i.first, vpi)) {
                    i.second->Set(1);
                }                   
                else {
                    i.second->Set(0);
                }
            }
            std::cout << "\n" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(gop->interval()));
        }       
    }
    catch (std::exception& e) {
        std::cout << "error: " << e.what() << std::endl;
    }

    return 0;
}

BOOL ListProcessModules(DWORD dwPID, std::vector<ProcessInfo>& vpi)
{
    HANDLE hProcess;
    PROCESSENTRY32 pe32;
    DWORD dwPriorityClass;
    HANDLE hProcessSnap;

    //  Take a snapshot of all modules in the specified process. 
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, dwPID);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
        return(FALSE);
    }
    pe32.dwSize = sizeof(PROCESSENTRY32);

    // Retrieve information about the first process,
    // and exit if unsuccessful
    if (!Process32First(hProcessSnap, &pe32))
    {
        printError(TEXT("Process32First")); // show cause of failure
        CloseHandle(hProcessSnap);          // clean the snapshot object
        return(FALSE);
    }

    // Now walk the snapshot of processes, and
    // display information about each process in turn
    do
    {       
        std::wstring ws(pe32.szExeFile);
        std::string str(ws.begin(), ws.end());
        std::stringstream ss;
        std::string Pid;
        ss << pe32.th32ProcessID;
        ss >> Pid;
        ProcessInfo pi;
        pi.set_moduleName(str);
        pi.set_pid(Pid);
        vpi.push_back(pi);       
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return(TRUE);
}
void printError(const TCHAR* msg)
{
    DWORD eNum;
    TCHAR sysMsg[256];
    TCHAR* p;

    eNum = GetLastError();
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, eNum,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        sysMsg, 256, NULL);

    // Trim the end of the line and terminate it with a null
    p = sysMsg;
    while ((*p > 31) || (*p == 9))
        ++p;
    do { *p-- = 0; } while ((p >= sysMsg) &&
        ((*p == '.') || (*p < 33)));
    // Display the message
    _tprintf(TEXT("\n  WARNING: %s failed with error %d (%s)"), msg, eNum, sysMsg);
}
void createMyGauges(Family<Gauge>& family, globalConfig* cfg, std::vector<std::pair<LabelType, Gauge*>>& vp) {
    vp.resize(cfg->cmdName()->size());
    for (auto i = 0; i < vp.size(); ++i) {
        vp.at(i).first = { "cmdName", cfg->cmdName()->at(i) };
        vp.at(i).second = &(family.Add({ vp.at(i).first }));
    }   
}
void getProcessInfo(std::vector<ProcessInfo>& vpi) {
    ListProcessModules(0, vpi);
}
bool isRunning(std::pair<std::string, std::string> label, const std::vector<ProcessInfo> &vpi) {
    for (auto i : vpi) {
        if (i.moduleName() == label.second) {
            std::cout << std::string(100, '-') << std::endl;
            std::cout << std::left << std::setw(64) << label.second << "\t\trunning\n" << std::endl;
            return true;
        }        
    }
    std::cout << std::string(100, '-') << std::endl;
    std::cout << std::left << std::setw(64) << label.second << "\t\tdead\n" << std::endl;
    return false;
}
