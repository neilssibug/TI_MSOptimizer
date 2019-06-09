select keyfil.device, keyfil.target_amount, keyfil.finished_amount, keyfil.shortage, keyfil.sol_qty sol_qty, 
sum(wip.cur_qty) wip_qty, round(keyfil.percent_error,2) percent_error 
from 
(
	select keydev_prod.device, keydev_prod.target_amount, keydev_prod.finished_amount, keydev_prod.shortage,sum(sol.quantity) sol_qty, ((abs(shortage)/target_amount)*100) percent_error 
    from 
	(
		SELECT device, target_amount, finished_amount, shortage, ((abs(shortage)/target_amount)*100) percent_error  
		from keydevice_production where ((abs(shortage)/target_amount)*100) > 10 
		group by device
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
) wip on keyfil.device = wip.device
group by keyfil.device
having sol_qty < wip_qty;