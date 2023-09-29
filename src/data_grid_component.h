#ifndef DATAGRIDCOMPONENT_H
#define DATAGRIDCOMPONENT_H

#include <godot_cpp/classes/node2d.hpp>
#include "data_grid_comp_ref.h"

namespace godot {

class DataGridCompRef;

class DataGridComponent : public Node2D {
	GDCLASS(DataGridComponent, Node2D)

private:
	Ref<DataGridCompRef> data_resource;
	
protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	DataGridComponent();
	~DataGridComponent();

	void set_data_resource(Ref<DataGridCompRef> p_data_resource);
	Ref<DataGridCompRef> get_data_resource();
	void set_layers(const TypedArray<int32_t> &p_layers);
	const Array &get_layers() const;
	void set_radius(float p_radius);
	float get_radius() const;
};
}

#endif // DATAGRIDCOMPONENT_H