#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <haru/Hpdf.h> // libHaru 头文件

using namespace std;

// 生成检测报告
string generateReport(int systolicPressure, int diastolicPressure) {
    stringstream report;
    report << "Blood Pressure Measurement:\n";
    report << "Systolic Pressure: " << systolicPressure << " mmHg\n";
    report << "Diastolic Pressure: " << diastolicPressure << " mmHg\n";
    // 在这里添加更多的信息，如检测结果和建议措施
    return report.str();
}

// 记录异常处理日志到文件
void logException(const exception& ex) {
    ofstream logFile("exception_log.txt", ios::app); // 使用追加模式
    if (!logFile.is_open()) {
        cerr << "Failed to open log file for writing." << endl;
        return;
    }
    logFile << "Exception: " << ex.what() << endl;
    logFile.close();
}

int main() {
    int systolicPressure = 120;
    int diastolicPressure = 80;

    // 生成报告内容
    string reportContent = generateReport(systolicPressure, diastolicPressure);

    // 使用 libHaru 创建 PDF 文档
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    if (!pdf) {
        cerr << "Error: Cannot create PDF document." << endl;
        return 1;
    }

    try {
        // 创建页面
        HPDF_Page page = HPDF_AddPage(pdf);
        if (!page) {
            throw runtime_error("Cannot add page to PDF document.");
        }

        // 设置字体
        HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
        if (!font) {
            throw runtime_error("Cannot load font.");
        }

        // 开始写入文本内容到 PDF 页面
        HPDF_Page_SetFontAndSize(page, font, 12);
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, 50, 750); // 设置文本位置

        // 将报告内容写入 PDF 页面
        HPDF_Page_ShowText(page, reportContent.c_str());

        // 结束写入文本
        HPDF_Page_EndText(page);

        // 保存 PDF 文件
        HPDF_SaveToFile(pdf, "report.pdf");

        // 释放资源
        HPDF_Free(pdf);

        cout << "PDF report has been generated successfully." << endl;

    } catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
        logException(ex); // 记录异常处理日志
        HPDF_Free(pdf); // 释放资源
        return 1;
    }

    return 0;
}
