extends Node2D


@onready var data_grid_manager: DataGridManager = $DataGridManager
@onready var timer: Timer = $DataGridTimer
@onready var grid_sprite: Sprite2D = $GridSprite


func _ready() -> void:
	timer.set_wait_time(data_grid_manager.get_update_frequency())
	timer.start()
	data_grid_manager.initialize_templates(1, 20, 1)


func _on_data_grid_timer_timeout() -> void:
	data_grid_manager.update()


func _on_data_grid_manager_updated(datagrid_collection: Object) -> void:
	grid_sprite._on_main_data_grid_updated(data_grid_manager.get_datagrid_size(),
	 datagrid_collection.get_layer_at_pos(Vector2(0, 0), 1).get_data())
