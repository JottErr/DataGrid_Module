extends Node2D

signal found_target (target_pos: Vector2)

@export var interest_grid : DataGrid
@export var interest_grid_size := Vector2i(31, 31)
@onready var think_cycle_timer: Timer = $ThinkCycleTimer

@onready var target_sprite: Sprite2D = $TargetSprite
@onready var grid_visualizer: Node2D = $GridVisualizer

var interest_template := DataGrid.new()


func _ready() -> void:
	var curve := MathCurve.new()
	curve.set_parameters(-1.0, 4, 0.0, 1.0)
	interest_template.set_cell_size(15)
	interest_template.set_size_in_cells(interest_grid_size)
	interest_template.radiate_value_at_position(interest_template.get_center(), interest_template.get_center().x, curve)
	
	think_cycle_timer.start()


func _on_think_cycle_timer_timeout() -> void:
	var flat_curve := MathCurve.new()
	flat_curve.set_parameters(1.0, 0.0, 0.0, 0.0)
	interest_grid = DataGrid.new()
	interest_grid.set_cell_size(15)
	interest_grid.set_size_in_cells(interest_grid_size)
	interest_grid.radiate_value_at_position(interest_grid.get_center(), 10, flat_curve)
	
	#add proximity layer of world datagrid to interest grid
	var world_grid_manager := DataGridHub.get_world_grid_manager()
	if world_grid_manager == null:
		print("no manager")
		return
	var datagrid_position := world_grid_manager.world_position_to_grid_position(global_position)
	var datagrid := world_grid_manager.get_datagrid_layer(datagrid_position, 1)
	
	if datagrid == null:
		return
	var pos_in_cell_cords := world_grid_manager.world_position_to_cell_in_data_grid(global_position, datagrid_position)
	
	interest_grid.add_from_pos_in_grid(datagrid, pos_in_cell_cords, -1.0)
	
	for y in interest_grid_size.y:
		for x in interest_grid_size.x:
			interest_grid.get_data()[x + y * interest_grid_size.x] *= interest_template.get_data()[x + y * interest_grid_size.x]
	
	var target: Vector2i = interest_grid.get_highest_cell()
	found_target.emit(Vector2(target - interest_grid.get_center()))
	var offset_of_interest_grid := snap_to_cell_center(global_position) - Vector2(interest_grid.get_center()) * 15 - Vector2(15.0 / 2.0, 15.0 / 2.0)
	target_sprite.global_position = get_world_pos_from_cell(Vector2i(), target) + offset_of_interest_grid
	
	var half_grid_size_pixel: Vector2 = interest_grid.get_center() * interest_grid.get_cell_size()
	grid_visualizer.global_position = snap_to_cell_center(global_position - half_grid_size_pixel) - Vector2(15.0 / 2.0, 15.0 / 2.0)
	grid_visualizer._on_data_grid_updated(interest_grid)


func snap_to_cell_center(pos : Vector2) -> Vector2:
	return Vector2(pos / 15.0).floor() * 15.0 + Vector2(15.0 / 2.0, 15.0 / 2.0)


func get_world_pos_from_cell(datagrid_position: Vector2i, cell_position: Vector2i) -> Vector2:
	var manager := DataGridHub.get_world_grid_manager()
	var position = cell_position * 15 + datagrid_position * manager.datagrid_size * 15
	return position
