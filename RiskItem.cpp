#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// リスク項目の構造体を定義
struct RiskItem {
    std::string name;
    std::string description;
    int severity;
};

// リスク項目をファイルに書き込む
void writeRiskItemsToFile(const std::vector<RiskItem>& riskItems, const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "リスク項目,説明,深刻さ\n";
        for (const auto& item : riskItems) {
            file << item.name << "," << item.description << "," << item.severity << "\n";
        }
        std::cout << "リスク項目がファイルに書き込まれました：" << filename << std::endl;
    } else {
        std::cerr << "ファイルを開けませんでした：" << filename << std::endl;
    }
}

int main() {
    // リスク項目リストの作成
    std::vector<RiskItem> riskItems = {
        {"手術リスク", "手術中に予期せぬ合併症が発生する可能性", 3},
        {"麻酔リスク", "麻酔中に合併症が発生する可能性", 2},
        {"術後リカバリーリスク", "手術後に回復が難しい可能性", 1}
    };

    // リスク項目をファイルに書き込む
    writeRiskItemsToFile(riskItems, "risk_assessment.csv");

    return 0;
}
