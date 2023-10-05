extends Node2D


@onready var data_grid_manager: DataGridManager = $DataGridManager
@onready var timer: Timer = $DataGridTimer


func _ready() -> void:
	DataGridHub.set_world_grid_manager(data_grid_manager)
	for wall in $Map.get_children():
		wall.register_in_manager(data_grid_manager)
	test_data_grid_manager()


func test_data_grid_manager() -> void:
	timer.set_wait_time(data_grid_manager.get_update_frequency())
	timer.start()
	data_grid_manager.initialize_templates(1, 20, 1)


func _on_data_grid_timer_timeout() -> void:
	data_grid_manager.update()


func test_performance() -> void:
	var testcurve := MathCurve.new()
	var data1 := DataGrid.new()
	data1.set_size_in_cells(Vector2i(9, 9))
	data1.radiate_value_at_position(Vector2i(4, 4), 4, testcurve, 1.0)
	var data2 := DataGrid.new()
	data2.set_size_in_cells(Vector2i(600, 600))
	for n in 10:
		var start = Time.get_ticks_usec()
		for i in 600:
			for j in 600:
				data2.add_grid_at_pos(data1, Vector2i(i, j), 1.0)
		print(Time.get_ticks_usec() - start)
