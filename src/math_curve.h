#ifndef MATHCURVE_H
#define MATHCURVE_H

#include <godot_cpp/classes/resource.hpp>

namespace godot {

class MathCurve : public Resource {
	GDCLASS(MathCurve, Resource)

private:
	float slope;
	float exponent;
	float x_shift;
	float y_shift;

protected:
	static void _bind_methods();

public:
	MathCurve();
	~MathCurve() {}

	float get_slope() const { return slope; }
	void set_slope(const float p_slope) { slope = p_slope; }
	float get_exponent() const { return exponent; }
	void set_exponent(const float p_exponent) { exponent = p_exponent; }
	float get_x_shift() const { return x_shift; }
	void set_x_shift(const float p_x_shift) { x_shift = p_x_shift; }
	float get_y_shift() const { return y_shift; }
	void set_y_shift(const float p_y_shift) { y_shift = p_y_shift; }

	float calculate_value(float x_value) const;
};

}

#endif // MATHCURVE_H