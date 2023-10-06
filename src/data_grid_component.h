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
	float time_since_registration;
	float registration_frequency;
	
protected:
	void _notification(int p_what);
	static void _bind_methods();

public:
	DataGridComponent();
	~DataGridComponent();

	void set_data_resource(Ref<DataGridCompRef> p_data_resource);
	Ref<DataGridCompRef> get_data_resource();
	void set_layer(int p_layer);
	int get_layer() const;
	void set_radius(float p_radius);
	float get_radius() const;
	void set_time_since_registration(int p_time);
	int get_time_since_registration() const;
	void set_registration_frequency(float p_freq);
	float get_registration_frequency() const;
	void _process(float p_delta);
};
}

#endif // DATAGRIDCOMPONENT_H