#ifndef DATAGRIDCOMPREF_H
#define DATAGRIDCOMPREF_H

#include "data_grid_component.h"

namespace godot {

class DataGridComponent;

class DataGridCompRef : public Resource {
	GDCLASS(DataGridCompRef, Resource);

private:
	DataGridComponent *component;
	
	TypedArray<int32_t> layers;
	float radius;
	bool registered;
	Vector2 registered_position;
	TypedArray<int32_t> registered_layers;
	float registered_radius;

protected:
	static void _bind_methods();

public:
	DataGridCompRef();
	~DataGridCompRef();

	void set_component(DataGridComponent *p_datagrid_component);
	DataGridComponent *get_component() const;
	bool component_is_valid() const;
	Vector2 get_global_position() const;
	void set_layers(const TypedArray<int32_t> &p_layers);
	const Array &get_layers() const;
	void set_radius(float p_radius);
	float get_radius() const;
	void set_registered(bool p_registered);
	bool is_registered() const;
	void set_registered_position(const Vector2 &p_position);
	const Vector2 &get_registered_position() const;
	void set_registered_layers(const TypedArray<int32_t> &p_layers);
	const Array &get_registered_layers() const;
	void set_registered_radius(float p_radius);
	float get_registered_radius() const;
};
}

#endif // DATAGRIDCOMPREF_H