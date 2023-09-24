extends Node2D


@onready var data_grid_manager: DataGridManager = $GridSprite/DataGridManager
@onready var timer: Timer = $DataGridTimer
@onready var grid_sprite: Sprite2D = $GridSprite


func _ready() -> void:
	timer.set_wait_time(data_grid_manager.get_update_frequency())
	timer.start()
	data_grid_manager.initialize_templates(1, 20, 1)


func _on_data_grid_timer_timeout() -> void:
	data_grid_manager.update()
