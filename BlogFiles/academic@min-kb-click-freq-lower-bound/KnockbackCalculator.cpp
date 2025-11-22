#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

double slip[2];
double mult;
double walkSpeed;
double springModifier;
double jumpMovementFactor;
double speedThreshold;

double clickD;

double vh[2];
double vy[2];

double s, h;

void init() {
	slip[0] = 1.0, slip[1] = 0.6;
	mult = 0.91;
	walkSpeed = 0.1;
	springModifier = 0.3;
	jumpMovementFactor = 0.02;
	speedThreshold = 0.005;
	clickD = 1.0;
	s = 0.0;
	h = 0.0;
}

void setInitialVelH(double val) { vh[0] = val; }
void setInitialVelY(double val) { vy[0] = val; }
void isClicking(bool val) { clickD = val ? 0.6 : 1.0; }
bool isOnGround() { return h <= 0; }

void tick0(double VelH = 0.0, double VelY = 0.3622, bool clicking = false) {
	setInitialVelH(VelH);
	setInitialVelY(VelY);
	isClicking(clicking);
	h = VelY;
}

signed main() {
	init();
	tick0(-0.8835, 0.3622, true); // spring
	// tick0(-0.8835 * 0.6, 0.3622, true); // not spring
	for(int ticks = 1; ; ++ticks) {
		int i = ticks % 2, j = (ticks + 1) % 2;
		bool onGround = isOnGround();

		if(ticks != 1 && vh[i] == vh[j] && onGround) break;

		vh[i] = vh[j] * slip[onGround] * mult;
		if(abs(vh[i]) < speedThreshold) vh[i] = 0.0; // perhaps it wasnt handled that way... but it doesnt matter
		// if(vh[i] >= 0) isClicking(false);
		vh[i] *= clickD;
		vh[i] += onGround ? walkSpeed * pow(0.6 / slip[1], 3) * (1.0 + springModifier) :
				jumpMovementFactor * (1.0 + springModifier);
		vh[i] *= 0.98;
		s += vh[i];

		if(!onGround) {
			vy[i] = (vy[j] - 0.08) * 0.98;
			if(abs(vy[i]) < speedThreshold) vy[i] = 0.0; // the same...
			h += vy[i];
			if(h < 0) h = 0;
		}

		cout << "[TICK " << ticks << "]";
		cout << " " << vh[i] << fixed << setprecision(6);
		// cout << " " << vh[i] * 20 << fixed << setprecision(6);

		cout << " " << s;

		cout << " " << (onGround ? "onGround" : "");
		cout << '\n';
	}
	return 0;
}