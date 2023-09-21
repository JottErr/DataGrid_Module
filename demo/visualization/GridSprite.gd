extends Sprite2D


func _on_main_data_grid_updated(size: Vector2i, data: Array) -> void:
	material.set_shader_parameter("grid_size", size)
	material.set_shader_parameter("data_grid", PackedFloat32Array(data))
