extends Node2D

@export var testcurve: MathCurve
@export var testgrid: DataGrid

@onready var data_grid_manager: DataGridManager = $DataGridManager
@onready var timer: Timer = $DataGridTimer
@onready var mouse_proximity: Node2D = $MouseProximity
@onready var grid_sprite: Sprite2D = $GridSprite


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	timer.set_wait_time(data_grid_manager.get_update_frequency())
	timer.start()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	mouse_proximity.set_global_position(get_global_mouse_position())


func _on_data_grid_timer_timeout() -> void:
	var grid = data_grid_manager.get_datagrid()
	
	for node in get_tree().get_nodes_in_group("ProximityComponents"):
		var prox_comp := node as ProximityComponent
		var radius = prox_comp.radius / data_grid_manager.get_cell_size()
		if prox_comp.registered:
			var last_cell := prox_comp.last_registered_position / data_grid_manager.get_cell_size()
			grid.radiate_value_at_position(last_cell, radius, testcurve, -1.0)
		
		var new_cell = Vector2i(prox_comp.get_global_position()) / data_grid_manager.get_cell_size()
		grid.radiate_value_at_position(new_cell, radius, testcurve)
		prox_comp.last_registered_position = prox_comp.get_global_position()
		prox_comp.registered = true
	
	grid_sprite._on_main_data_grid_updated(data_grid_manager.get_datagrid_size(), grid.get_data())
