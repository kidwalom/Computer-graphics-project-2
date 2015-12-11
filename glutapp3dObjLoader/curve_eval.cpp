#include "curve_eval.h"

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_poly.h"
# include "math.h"

#include <vector>

double choose(float n, float k) {
	//return ((factorial(n)) / (factorial(k)*factorial(n - k)));
	if (k > n) {
		return 0;
	}
	if (k * 2 > n) {
		k = n - k;
	}
	if (k == 0) {
		return 1;
	}

	int result = n;
	for (int i = 2; i <= k; ++i) {
		result *= (n - i + 1);
		result /= i;
	}
	return result;
}


void eval_bezier(int n, GsArray<GsVec>& curve, const GsArray<GsVec>& ctrlpnts) {
	int size = ctrlpnts.size();
	curve.size(n);

	std::vector<float> const_t(size);

	for (int t = 0; t < n; t++) {
		float f = float(t) / float(n - 1);
		GsVec y(0, 0, 0);

		for (int i = 0; i <= size; i++) {
			float bezier = float(choose(size, i)*powf(f, i)*powf(1 - f, size - i));
			y += ctrlpnts[i] * bezier;
		}
		curve[t] = y;
	}
}


void eval_lagrange(int n, GsArray<GsVec>& curve, const GsArray<GsVec>& ctrlpnts) {
	const int size = ctrlpnts.size();
	curve.size(n);

	std::vector<float> const_t(size);

	for (int i = 0; i < size; i++) {
		float k = float(i) / float(size - 1);
		const_t[i] = k;
	}
	for (int t = 0; t < n; t++) {
		float x = ((float)t / (float)(n - 1));
		GsVec y(0.0, 0.0, 0.0);
		for (int i = 0; i < size; i++) {
			GsVec point = ctrlpnts[i];
			float b = 1.0f;
			for (int j = 0; j < size; j++) {
				if (j == i) {
					continue;
				}

				//GsVec point_j = ctrlpnts[j];
				b *= ((x - const_t[j]) / (const_t[i] - const_t[j]));

			}
			y += point*b;
		}
		curve[t] = y;
	}
}

GsVec eval_bsspline(int n, int k, float t, const GsArray<GsVec>& ctrlpnts) {
	//const int size = ctrlpnts.size();
	//const_t[i] =
	GsVec x(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < n; i++) {
		x += N(i, k, t) * ctrlpnts[i];
	}
	return x;
}

GsVec eval_crspline(float t, int n, const GsArray<GsVec>& ctrlpnts) {
	int i = floor(t);
	GsArray<GsVec> b;
	GsVec I = (ctrlpnts[i + 1] - ctrlpnts[i - 1]) / 2.0f;
	GsVec p_i_minus = ctrlpnts[i] - I / 3.0f;
	GsVec p_i_plus = ctrlpnts[i] + I / 3.0f;

	b.push(ctrlpnts[i]); //p_i
	b.push(p_i_plus); //p_i+

	GsVec I2 = (ctrlpnts[i + 2] - ctrlpnts[i]) / 2.0f;
	GsVec p_i_plus_1_minus = ctrlpnts[i + 1] - I2 / 3.0f;
	b.push(p_i_plus_1_minus); // p_i+1_-
	b.push(ctrlpnts[i + 1]);

	//GsVec point(0.0f, 0.0f, 0.0f);

	/*float t = float(i) / float(n);
	return evalbezier(t, b)
	point = (b[0] + t*b[1] + t*t*b[2] + t*t*t*b[3])*0.5f;*/

	return eval_bezier(t - i, b);

	/*GsVec P(0.0f, 0.0f, 0.0f);

	P = 0.5*(b[0] + t*b[1] + t*t*b[2] + t*t*t*b[3]);
	return P;*/


	/*for (int i = 1; i < n-2; i++) {
	GsVec temp(0.0f, 0.0f, 0.0f);
	GsArray<GsVec> b;

	GsVec p_i_prev = ctrlpnts[i - 1];
	GsVec p_i_next = ctrlpnts[i + 1];
	GsVec I_i = (p_i_next - p_i_prev) / 2;
	GsVec p_i_minus = ctrlpnts[i] - I_i / 3.0f;
	GsVec p_i_plus = ctrlpnts[i] + I_i / 3.0f;
	b.push(p_i_prev);
	b.push(p_i_next);
	b.push(p_i_minus);
	b.push(p_i_plus);

	for (int t = 0; t < n; t++) {
	float f = float(t) / float(n - 1);

	for (int i = 0; i <= 4; i++) {
	float bezier = float(choose(4, i)*powf(f, i)*powf(1 - f, 4 - i));
	y += ctrlpnts[i] * bezier;
	}
	curve[t] = y;
	}

	}*/
}

GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts) {
	int size = ctrlpnts.size() - 1;
	//curve.size(n);

	//float f = float(t) / float(n - 1);
	GsVec y(0, 0, 0);

	for (int i = 0; i <= size; i++) {
		float bezier = float(choose(size, i)*powf(t, i)*powf(1 - t, size - i));
		y += ctrlpnts[i] * bezier;
	}
	return y;
}

GsVec eval_bospline(float t, const GsArray<GsVec>& ctrlpnts) {
	int i = floor(t);
	GsArray<GsVec> points;

	//i
	float dist_i_minus = dist(ctrlpnts[i], ctrlpnts[i - 1]);
	float dist_i_plus = dist(ctrlpnts[i + 1], ctrlpnts[i]);
	GsVec v_hat_plus = (ctrlpnts[i + 1] - ctrlpnts[i]) / dist_i_plus;
	GsVec v_hat_minus = (ctrlpnts[i] - ctrlpnts[i - 1]) / dist_i_minus;
	GsVec v = (dist_i_plus*v_hat_minus + dist_i_minus*v_hat_plus) / (dist_i_plus + dist_i_minus);
	points.push(ctrlpnts[i]);
	GsVec p_i_plus = ctrlpnts[i] + (dist_i_plus*v) / 3.0;
	points.push(p_i_plus);

	//i+1
	/*float dist_i_plus_one_minus = dist(ctrlpnts[i+1], ctrlpnts[i]);
	float dist_i_plus_one_plus = dist(ctrlpnts[i + 2], ctrlpnts[i+1]);
	GsVec v_hat_i_plus_one_plus = (ctrlpnts[i + 2] - ctrlpnts[i+1]) / dist_i_plus_one_plus;
	GsVec v_hat_i_plus_one_minus = (ctrlpnts[i+1] - ctrlpnts[i]) / dist_i_plus_one_minus;
	GsVec v_i_plus_one = (dist_i_plus_one_plus*v_hat_i_plus_one_minus + dist_i_plus_one_minus*v_hat_i_plus_one_plus) / (dist_i_plus_one_plus + dist_i_plus_one_minus);
	GsVec p_i_plus_one_minus = ctrlpnts[i + 1] - (dist_i_plus_one_minus*v_i_plus_one) / 3.0;
	points.push(p_i_plus_one_minus);
	points.push(ctrlpnts[i+1]);*/

	dist_i_minus = dist(ctrlpnts[i + 1], ctrlpnts[i]);
	dist_i_plus = dist(ctrlpnts[i + 2], ctrlpnts[i + 1]);
	v_hat_plus = (ctrlpnts[i + 2] - ctrlpnts[i + 1]) / dist_i_plus;
	v_hat_minus = (ctrlpnts[i + 1] - ctrlpnts[i]) / dist_i_minus;
	v = (dist_i_plus*v_hat_minus + dist_i_minus*v_hat_plus) / (dist_i_plus + dist_i_minus);

	GsVec p_i_minus = ctrlpnts[i + 1] - (dist_i_minus*v) / 3.0;
	points.push(p_i_minus);
	points.push(ctrlpnts[i + 1]);

	/*GsVec p_i_plus = ctrlpnts[i] - (dist_i_plus*v) / 3.0;
	GsVec p_i_plus_one_minus = ctrlpnts[i+1] - (dist_i_minus*v) / 3.0;
	points.push(p_i_plus);
	points.push(p_i_plus_one_minus);
	points.push(ctrlpnts[i + 1]);*/

	GsVec k(0.0, 0.0, 0.0);
	k = eval_bezier(t - i, points);
	return k;
}

static float N(int i, int k, float u) {
	float ui = float(i);
	if (k == 1) {
		return ui <= u && u < ui + 1 ? 1.0f : 0;
	}
	else {
		return ((u - ui) / (k - 1))*N(i, k - 1, u) + ((ui + k - u) / (k - 1))*N(i + 1, k - 1, u);
	}
}