#ifndef DATAGRIDCOMPONENT_H
#define DATAGRIDCOMPONENT_H

#include <godot_cpp/classes/node2d.hpp>

namespace godot {

class DataGridComponent : public Node2D {
	GDCLASS(DataGridComponent, Node2D)

private:
	TypedArray<int32_t> layers; //bitfield? enum?
	float radius;
	
	bool registered;
	Vector2 registered_position;
	TypedArray<int32_t> registered_layers;
	float registered_radius;
	
protected:
	static void _bind_methods();

public:
	DataGridComponent();
	~DataGridComponent();
	
	void set_layers(const TypedArray<int32_t> &p_layers) { layers = p_layers; }
	const Array &get_layers() const { return layers; } // *1
	void set_radius(float p_radius) { radius = p_radius; }
	float get_radius() const { return radius; }
	void set_registered(bool p_registered) { registered = p_registered; }
	bool is_registered() const { return registered; }
	void set_registered_position(const Vector2 &p_position) { registered_position = p_position; }
	const Vector2 &get_registered_position() const { return registered_position; }
	void set_registered_layers(const TypedArray<int32_t> &p_layers) { registered_layers = p_layers; }
	const Array &get_registered_layers() const { return registered_layers; } // *1
	void set_registered_radius(float p_radius) { registered_radius = p_radius; }
	float get_registered_radius() const { return registered_radius; }

	void _enter_tree();
};
}
// *1: cant return TypedArray<int32_t> binder_common.hpp(209): error C2039: "encode" ist kein Member von "godot::PtrToArg<R>".
#endif // DATAGRIDCOMPONENT_H