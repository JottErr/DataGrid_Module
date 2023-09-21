extends Sprite2D


@onready var data_grid_component: DataGridComponent = $DataGridComponent


func _process(delta: float) -> void:
	set_global_position(get_global_mouse_position())

func _unhandled_input(event: InputEvent) -> void:
	if Input.is_action_just_pressed("scroll_down"):
		data_grid_component.radius -= 15
		print(data_grid_component.radius)
	if Input.is_action_just_pressed("scroll_up"):
		data_grid_component.radius += 15
		print(data_grid_component.radius)
