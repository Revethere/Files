#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>

class entity {
    bool on_ground[2], delay;
    double s, h;
    double motion_h, motion_y;
    double click_sd;
    
public:
    bool   isOnGround()                        { return h <= 0; }
    double isClicking(const bool &val)         { return click_sd = val ? 0.6 : 1.0; }
    double getMotionH()                        { return motion_h; }
    double getSpatium()                        { return s; }
    void move(const int &ticks, const bool &i) {
        static constexpr double mult = 0.91,
                                spring_modifier = 0.3,
                                // walkSpeed = 0.1,
                                // speedThreshold = 0.05,
                                // jumpMovementFactor = 0.02;
                                resist_coeff[] = {0.02, 0.05, 0.1};
        static constexpr double slip[] = {1.0, 0.6};
        static constexpr double speed_threshold = 0.005;

        on_ground[i] = isOnGround();
        if(motion_h >= 0) isClicking(false);

        if(delay) delay = false; 
        else motion_h *= mult * slip[on_ground[!i]];
        motion_h *= click_sd;
        if(abs(motion_h) < speed_threshold) motion_h = 0.0;

        motion_h += (on_ground[i] ?
                    resist_coeff[2] * pow(0.6 / slip[1], 3) :
                    resist_coeff[0]
                    ) * (1.0 + spring_modifier) * 0.98;
        s += motion_h;

        if(!on_ground[i]) {
            motion_y -= 0.08;
            motion_y *= 0.98;
            if(abs(motion_y) < speed_threshold) motion_y = 0.0;
        }
        h += motion_y;
        if(h < 0.0) h = 0.0;
    }

    bool setVelocity(const double &motion_h = 0.0, const double &motion_y = 0.0, const bool &is_clicking = false) {
        this->motion_h = motion_h, this->motion_y = motion_y;
        isClicking(is_clicking);
        delay = true;
        return true;
    }

    entity(const double &motion_h = 0.0, const double &motion_y = 0.0, const bool &on_ground = true, const bool &is_clicking = false): s(0.0), h(0.0), on_ground{on_ground} { setVelocity(motion_h, motion_y, is_clicking); }
};

auto buildFileName() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm *local = std::localtime(&t);

    std::ostringstream oss;
    oss << "result_" << std::put_time(local, "%m%d_%H%M") << ".csv";
    return oss;
}

using namespace std;

signed main() {
    ofstream file(buildFileName().str());

    entity player(-0.949375, 0.3622, true, true);

    double motion_h[2] = {0.0, 0.0};
    for(int ticks = 1; ; ++ticks) {
        const bool i = ticks & 1, j = !i;
        player.move(ticks, i);
        if((motion_h[i] = player.getMotionH()) == motion_h[j] && ticks != 1 && player.isOnGround()) break;
        
        file << "tick " << ticks << "," << motion_h[i] << "," << player.getSpatium() << "\n";
        // cout << "tick " << ticks << "," << motion_h[i] << "," << player.getSpatium() << "\n";
    }

    file.close();
    return 0;
}