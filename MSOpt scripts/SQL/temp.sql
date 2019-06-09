select keyfil.device, keyfil.qty, sum(wip.cur_qty) from 
(
	select device, sum(quantity) as qty from 
		(
			SELECT device,((abs(shortage)/target_amount)*100) as percent_error  from keydevice_production where ((abs(shortage)/target_amount)*100) > 10 group by device
		) keydev_prod
	inner join 
	(
		select * from solution
	) sol on keydev_prod.device = sol.device_name
	group by keydev_prod.device
) keyfil
inner join
(
	select * from wip
) wip on keyfil.device = wip.device and keyfil.device = '603V1RUNR-G2N#'
group by keyfil.device;
