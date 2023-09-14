#include "math_curve.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void MathCurve::_bind_methods() { 
	ClassDB::bind_method(D_METHOD("get_slope"), &MathCurve::get_slope);
	ClassDB::bind_method(D_METHOD("set_slope", "slope"), &MathCurve::set_slope);
	ClassDB::bind_method(D_METHOD("get_exponent"), &MathCurve::get_exponent);
	ClassDB::bind_method(D_METHOD("set_exponent", "exponent"), &MathCurve::set_exponent);
	ClassDB::bind_method(D_METHOD("get_x_shift"), &MathCurve::get_x_shift);
	ClassDB::bind_method(D_METHOD("set_x_shift", "x_shift"), &MathCurve::set_x_shift);
	ClassDB::bind_method(D_METHOD("get_y_shift"), &MathCurve::get_y_shift);
	ClassDB::bind_method(D_METHOD("set_y_shift", "y_shift"), &MathCurve::set_y_shift);

	ClassDB::bind_method(D_METHOD("calculate_value", "x_value"), &MathCurve::calculate_value);

	ClassDB::add_property("MathCurve", PropertyInfo(Variant::FLOAT, "slope"), "set_slope", "get_slope");
	ClassDB::add_property("MathCurve", PropertyInfo(Variant::FLOAT, "exponent"), "set_exponent", "get_exponent");
	ClassDB::add_property("MathCurve", PropertyInfo(Variant::FLOAT, "x_shift"), "set_x_shift", "get_x_shift");
	ClassDB::add_property("MathCurve", PropertyInfo(Variant::FLOAT, "y_shift"), "set_y_shift", "get_y_shift");
}

MathCurve::MathCurve(){
	slope = 1.0;
	exponent = 1.0;
	x_shift = 0.0;
	y_shift = 0.0;
}

float MathCurve::calculate_value(float x_value) const {
	return slope * Math::pow((x_value + x_shift), exponent) + y_shift;
}