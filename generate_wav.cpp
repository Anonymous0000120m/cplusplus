#include <iostream>
#include <sndfile.h>
#include <cmath>
#include <vector>

class SineWaveGenerator {
public:
    SineWaveGenerator(double frequency, double duration, int sample_rate)
        : frequency(frequency), duration(duration), sample_rate(sample_rate) {
        num_samples = static_cast<int>(duration * sample_rate);
        signal.resize(num_samples);
    }

    void generateSignal() {
        for (int i = 0; i < num_samples; ++i) {
            double t = static_cast<double>(i) / sample_rate; // 计算时间
            signal[i] = static_cast<short>(std::sin(2 * M_PI * frequency * t) * 32767); // 生成信号
        }
    }

    bool saveToWAV(const std::string& filename) {
        SF_INFO sfinfo;
        sfinfo.frames = num_samples;
        sfinfo.samplerate = sample_rate;
        sfinfo.channels = 1; // 单声道
        sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16; // WAV格式，16位PCM

        SNDFILE* outfile = sf_open(filename.c_str(), SFM_WRITE, &sfinfo);
        if (!outfile) {
            std::cerr << "Error opening output file: " << sf_strerror(NULL) << std::endl;
            return false;
        }

        sf_writef_short(outfile, signal.data(), num_samples);
        sf_close(outfile);
        return true;
    }

private:
    double frequency;
    double duration;
    int sample_rate;
    int num_samples;
    std::vector<short> signal;
};

int main() {
    double frequency = 440.0; // 频率（Hz）
    double duration = 5.0;     // 时长（秒）
    int sample_rate = 44100;   // 采样率

    SineWaveGenerator generator(frequency, duration, sample_rate);
    generator.generateSignal();

    if (generator.saveToWAV("output.wav")) {
        std::cout << "WAV file generated successfully: output.wav" << std::endl;
    }

    return 0;
}
