extends Sprite2D

@export var c_offset : Vector2
@onready var data_grid_component: DataGridComponent = $DataGridComponent


func _process(delta: float) -> void:
	set_global_position(get_global_mouse_position() + c_offset)


func _unhandled_input(event: InputEvent) -> void:
	if Input.is_action_just_pressed("scroll_down"):
		data_grid_component.set_radius(data_grid_component.get_radius() - DataGridHub.get_cell_size())
		print(data_grid_component.get_radius())
	if Input.is_action_just_pressed("scroll_up"):
		data_grid_component.set_radius(data_grid_component.get_radius() + DataGridHub.get_cell_size())
		print(data_grid_component.get_radius())
