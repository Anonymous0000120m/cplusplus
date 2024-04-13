/*
================================================================================
Copyright (c) 2024 [Vrml Studio Inc]. All rights reserved.

This software is provided "AS IS" without any warranty of any kind, express or
implied, including but not limited to the implied warranties of merchantability
and fitness for a particular purpose. In no event shall the authors or
copyright holders be liable for any direct, indirect, incidental, special,
exemplary, or consequential damages (including, but not limited to, procurement
of substitute goods or services; loss of use, data, or profits; or business
interruption) however caused and on any theory of liability, whether in contract,
strict liability, or tort (including negligence or otherwise) arising in any way
out of the use of this software, even if advised of the possibility of such damage.

For more information, please refer to the license agreement.
================================================================================
*/

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// 画像のサイズと強度の範囲を定義
const int IMAGE_SIZE = 100;
const int INTENSITY_RANGE = 256;

// 個体を表すクラス
class Individual {
public:
    std::vector<int> intensity_map; // 個体のレーザー強度画像を格納する

    // コンストラクタ、ランダムに個体を生成
    Individual() {
        for (int i = 0; i < IMAGE_SIZE * IMAGE_SIZE; ++i) {
            intensity_map.push_back(rand() % INTENSITY_RANGE);
        }
    }

    // 個体の適応度（レーザー強度の合計）を計算する
    int fitness() const {
        int total_intensity = 0;
        for (int intensity : intensity_map) {
            total_intensity += intensity;
        }
        return total_intensity;
    }
};

// 遺伝的アルゴリズムクラスの定義
class GeneticAlgorithm {
public:
    // 選択操作、ルーレット選択法を使用して2つの親個体を選択する
    static std::pair<Individual, Individual> selection(const std::vector<Individual>& population) {
        int total_fitness = 0;
        for (const Individual& individual : population) {
            total_fitness += individual.fitness();
        }

        std::pair<Individual, Individual> parents;
        for (int i = 0; i < 2; ++i) {
            int target = rand() % total_fitness;
            int sum = 0;
            for (const Individual& individual : population) {
                sum += individual.fitness();
                if (sum >= target) {
                    parents.first = individual;
                    break;
                }
            }
        }
        return parents;
    }

    // 交叉操作、単一交叉を使用して2つの子個体を生成する
    static std::pair<Individual, Individual> crossover(const Individual& parent1, const Individual& parent2) {
        int crossover_point = rand() % (IMAGE_SIZE * IMAGE_SIZE);
        Individual child1, child2;
        for (int i = 0; i < IMAGE_SIZE * IMAGE_SIZE; ++i) {
            if (i < crossover_point) {
                child1.intensity_map[i] = parent1.intensity_map[i];
                child2.intensity_map[i] = parent2.intensity_map[i];
            } else {
                child1.intensity_map[i] = parent2.intensity_map[i];
                child2.intensity_map[i] = parent1.intensity_map[i];
            }
        }
        return {child1, child2};
    }

    // 突然変異操作、個体の一部の遺伝子をランダムに変異させる
    static void mutate(Individual& individual, double mutation_rate) {
        for (int i = 0; i < IMAGE_SIZE * IMAGE_SIZE; ++i) {
            if ((double) rand() / RAND_MAX < mutation_rate) {
                individual.intensity_map[i] = rand() % INTENSITY_RANGE;
            }
        }
    }
};

// メイン関数、遺伝的アルゴリズムの実行エントリポイント
int main() {
    // 乱数シードの設定
    srand(time(NULL));

    // 集団の初期化
    const int POPULATION_SIZE = 100;
    std::vector<Individual> population(POPULATION_SIZE);

    // 繰り返し回数
    const int NUM_GENERATIONS = 100;

    // 遺伝的アルゴリズムのメインループ
    for (int generation = 0; generation < NUM_GENERATIONS; ++generation) {
        // 選択、交叉、突然変異操作
        std::vector<Individual> next_generation;
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            std::pair<Individual, Individual> parents = GeneticAlgorithm::selection(population);
            std::pair<Individual, Individual> children = GeneticAlgorithm::crossover(parents.first, parents.second);
            GeneticAlgorithm::mutate(children.first, 0.01);
            GeneticAlgorithm::mutate(children.second, 0.01);
            next_generation.push_back(children.first);
            next_generation.push_back(children.second);
        }
        population = next_generation;
    }

    // 最適な個体の適応度を出力
    int max_fitness = 0;
    Individual best_individual;
    for (const Individual& individual : population) {
        int current_fitness = individual.fitness();
        if (current_fitness > max_fitness) {
            max_fitness = current_fitness;
            best_individual = individual;
        }
    }
    std::cout << "Best fitness: " << max_fitness << std::endl;

    return 0;
}
