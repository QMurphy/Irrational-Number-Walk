#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <fstream>

cv::Scalar hsv2bgr(cv::Scalar hsv) {
	hsv[0] = hsv[0] / 30.0;
	int i = static_cast<int>(std::floor(hsv[0]));
	double f = hsv[0] - i;
	int p = hsv[2] * (1.0 - hsv[1] / 255.0);
	int q = hsv[2] * (1.0 - (hsv[1] / 255.0 * f));
	int t = hsv[2] * (1.0 - (hsv[1] / 255.0 * (1.0 - f)));
	switch (i) {
	case 0: return cv::Scalar(p, t, hsv[2]);
	case 1: return cv::Scalar(p, hsv[2], q);
	case 2: return cv::Scalar(t, hsv[2], p);
	case 3: return cv::Scalar(hsv[2], q, p);
	case 4: return cv::Scalar(hsv[2], p, t);
	case 5: default:return cv::Scalar(q, p, hsv[2]);
	}
}

int main(int argc, char** argv) {
	int width = 4800;
	int height = 6000;
	double x, prevX, y, prevY;
	double mult;
	int N = 1000000;
	
	std::fstream in("E:/Dropbox/Pi Walk/pi10.html", std::ios_base::in);
	x = prevX = 1300;
	y = prevY = 800;
	mult = 4;

	/*std::fstream in("E:/Dropbox/Pi Walk/sqrt2.txt", std::ios_base::in);
	x = prevX = 3100;
	y = prevY = 600;
	mult = 4.5; */

	/*std::fstream in("E:/Dropbox/Pi Walk/phi.txt", std::ios_base::in);
	x = prevX = 3950;
	y = prevY = 1200;
	mult = 3;*/

	/*std::fstream in("E:/Dropbox/Pi Walk/e.txt", std::ios_base::in);
	x = prevX = 1200;
	y = prevY = 200;
	mult = 3; */

	cv::Mat im(cv::Size(width, height), CV_8UC3);
	im.setTo(cv::Scalar::all(255));
	cv::Scalar hsv, bgr;
	cv::namedWindow("Pic", cv::WINDOW_FREERATIO);

	int count = 0;
	for (int i = 0; i < N; i++) {
		char c = in.get();
		count++;
		while (c < '0' || c > '9') {
			if (in.eof()) {
				c = '0';
				break;
			}
			else if (c == ':') {
				while (in.get() != '\n' && !in.eof());
			}
			c = in.get();
			count++;
		}

		int h = i * 130 / N - 15;
		hsv = cv::Scalar(h < 0 ? h + 180 : h, 255, 255);
		bgr = hsv2bgr(hsv);

		x += std::sin((c - '0') * 2 * 3.141592654 / 10) * mult;
		y -= std::cos((c - '0') * 2 * 3.141592654 / 10) * mult;
		
		// Draw
		cv::line(im, cv::Point(x, y), cv::Point(prevX, prevY), bgr, 2);

		prevX = x;
		prevY = y;

		if (in.eof()) {
			break;
		}
	}

	cv::imshow("Pic", im);
	if (cv::waitKey(0) == 27) {
		return 0;
	}
	cv::imwrite("E:/Dropbox/Pi Walk/pi.png", im);

	return 0;
}