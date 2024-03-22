// processwindow.cpp
#include "processwindow.h"
#include "ui_processwindow.h"

#include <windows.h>
#include <tlhelp32.h>
#include <QString>
#include <psapi.h>

processwindow::processwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::processwindow)
{
    ui->setupUi(this);
}

processwindow::~processwindow()
{
    delete ui;
}

void processwindow::populateProcessList() {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        // Handle error
        return;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hSnapshot, &pe32)) {
        // Handle error
        CloseHandle(hSnapshot);
        return;
    }

    ui->processListWidget->clear(); // Assuming you have a QTableWidget named processListWidget in your UI
    ui->processListWidget->setRowCount(0); // Remove all rows


    // Set column count and headers
    ui->processListWidget->setColumnCount(5);
    ui->processListWidget->setHorizontalHeaderLabels(QStringList() << "Process ID" << "Process Name" << "Memory Usage" << "Thread Count" << "User Name" << "Start Time");

    do {
        QString processName = QString::fromWCharArray(pe32.szExeFile);
        QString processID = QString::number(pe32.th32ProcessID);

        // Open a handle to the process
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
        if (hProcess) {
            // Create a new row
            int row = ui->processListWidget->rowCount();
            ui->processListWidget->insertRow(row);

            // Add items to the row
            ui->processListWidget->setItem(row, 0, new QTableWidgetItem(processID));
            ui->processListWidget->setItem(row, 1, new QTableWidgetItem(processName));

            // Get memory usage
            PROCESS_MEMORY_COUNTERS pmc;
            if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                SIZE_T memoryUsageBytes = pmc.WorkingSetSize; // This is the working set size, in bytes

                double memoryUsageKB = memoryUsageBytes / 1024.0; // Convert to kilobytes
                double memoryUsageMB = memoryUsageKB / 1024.0; // Convert to megabytes
                double memoryUsageGB = memoryUsageMB / 1024.0; // Convert to gigabytes

                QString memoryUsageStr;
                if (memoryUsageGB > 1.0) {
                    memoryUsageStr = QString::number(memoryUsageGB, 'f', 2) + " GB";
                } else if (memoryUsageMB > 1.0) {
                    memoryUsageStr = QString::number(memoryUsageMB, 'f', 2) + " MB";
                } else if (memoryUsageKB > 1.0) {
                    memoryUsageStr = QString::number(memoryUsageKB, 'f', 2) + " KB";
                } else {
                    memoryUsageStr = QString::number(memoryUsageBytes) + " Bytes";
                }

                // Add items to the row
                ui->processListWidget->setItem(row, 2, new QTableWidgetItem(memoryUsageStr));
            }

            // Get start time
            FILETIME creationTime, exitTime, kernelTime, userTime;
            if (GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime)) {
                // Convert the FILETIME to SYSTEMTIME
                SYSTEMTIME systemTime;
                FileTimeToSystemTime(&creationTime, &systemTime);

                // Format the SYSTEMTIME as a string
                QString startTime = QString::asprintf("%02d/%02d/%04d %02d:%02d:%02d",
                                                      systemTime.wMonth, systemTime.wDay, systemTime.wYear,
                                                      systemTime.wHour, systemTime.wMinute, systemTime.wSecond);

                // Add the start time to the table
                ui->processListWidget->setItem(row, 5, new QTableWidgetItem(startTime));
            }


            // Get thread count
            DWORD threadCount = pe32.cntThreads;

            // Convert threadCount to QString
            QString threadCountStr = QString::number(threadCount);

            // Add thread count to the table
            ui->processListWidget->setItem(row, 3, new QTableWidgetItem(threadCountStr));

            // Get user name
            HANDLE hToken;
            if (OpenProcessToken(hProcess, TOKEN_QUERY, &hToken)) {
                DWORD dwBufferSize = 0;
                GetTokenInformation(hToken, TokenUser, NULL, 0, &dwBufferSize);
                PTOKEN_USER pTokenUser = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBufferSize);
                if (pTokenUser) {
                    if (GetTokenInformation(hToken, TokenUser, pTokenUser, dwBufferSize, &dwBufferSize)) {
                        WCHAR szUserName[256];
                        DWORD dwUserNameLength = 256;
                        WCHAR szDomainName[256];
                        DWORD dwDomainNameLength = 256;
                        SID_NAME_USE sidType;
                        if (LookupAccountSidW(NULL, pTokenUser->User.Sid, szUserName, &dwUserNameLength, szDomainName, &dwDomainNameLength, &sidType)) {
                            QString userName = QString::fromWCharArray(szUserName);

                            // Add user name to the table
                            ui->processListWidget->setItem(row, 4, new QTableWidgetItem(userName));
                        }
                    }
                    HeapFree(GetProcessHeap(), 0, pTokenUser);
                }
                CloseHandle(hToken);
            }

            CloseHandle(hProcess);
        }
    }while (Process32Next(hSnapshot, &pe32));

    CloseHandle(hSnapshot);

}

void processwindow::showEvent(QShowEvent *event) {
    QDialog::showEvent(event); // Call base class implementation
    populateProcessList(); // Update the process list
}

void processwindow::on_refreshButton_clicked() {
    populateProcessList();
}
