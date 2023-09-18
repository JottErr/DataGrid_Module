extends Node2D

@export var testcurve: MathCurve
@export var testgrid: DataGrid
@export var testgrid2: DataGrid

@onready var data_grid_manager: DataGridManager = $DataGridManager
@onready var timer: Timer = $DataGridTimer
@onready var mouse_proximity: Node2D = $MouseProximity
@onready var grid_sprite: Sprite2D = $GridSprite


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	timer.set_wait_time(data_grid_manager.get_update_frequency())
	timer.start()
	data_grid_manager.initialize_templates(1, 21, 1)

func _unhandled_input(event: InputEvent) -> void:
	if Input.is_action_just_pressed("scroll_down"):
		mouse_proximity.radius -= data_grid_manager.get_cell_size()
		print(mouse_proximity.radius)
	if Input.is_action_just_pressed("scroll_up"):
		mouse_proximity.radius += data_grid_manager.get_cell_size()
		print(mouse_proximity.radius)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	mouse_proximity.set_global_position(get_global_mouse_position())


func _on_data_grid_timer_timeout() -> void:
	var grid = data_grid_manager.get_datagrid()
	
	for node in get_tree().get_nodes_in_group("ProximityComponents"):
		var prox_comp := node as ProximityComponent
		if prox_comp == null: 
			continue
		
		if prox_comp.registered:
			var last_cell := prox_comp.last_registered_position / data_grid_manager.get_cell_size()
			var last_radius := prox_comp.last_registered_radius / data_grid_manager.get_cell_size()
			grid.add_grid_at_pos(data_grid_manager.get_template(last_radius), last_cell, -1.0)
		
		var new_cell = Vector2i(prox_comp.get_global_position()) / data_grid_manager.get_cell_size()
		var new_radius = prox_comp.radius / data_grid_manager.get_cell_size()
		grid.add_grid_at_pos(data_grid_manager.get_template(new_radius), new_cell, 1.0)
		prox_comp.last_registered_position = prox_comp.get_global_position()
		prox_comp.last_registered_radius = prox_comp.radius
		prox_comp.registered = true
	
	grid_sprite._on_main_data_grid_updated(data_grid_manager.get_datagrid_size(), grid.get_data())
