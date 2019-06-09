select now();

select sum(cur_qty) from wip where device='NA826AIDRGR-RK';

select * from solution where device_name = 'NA826AIDRGR-RK';

select keyfil.device, keyfil.target_amount, keyfil.finished_amount, keyfil.shortage, keyfil.sol_qty sol_qty, 
ifnull(sum(wip.cur_qty),0) wip_qty, round(keyfil.percent_error,2) percent_error,
case
	when ifnull(sum(wip.cur_qty),0) <= 0 then 'no wip'
	when ifnull(sum(wip.cur_qty),0) >= keyfil.target_amount
    then
		case 
        when keyfil.finished_amount <= 0 then 'wip_qty greater or equal to target_amount; no finished_amount'
        else 
			case 
			when keyfil.finished_amount < keyfil.target_amount then 'wip_qty greater or equal to target_amount; finished_amount less than target_amount'
            end
        end
	when ifnull(sum(wip.cur_qty),0) < keyfil.target_amount
    then
		case 
        when keyfil.finished_amount <= 0 then 'wip_qty less than target_amount; no finished_amount'
        else 
			case 
			when keyfil.finished_amount <= keyfil.target_amount then 'wip_qty less than target_amount; finished_amount less than target_amount'
            else 'lvl2. no remarks'
            end
        end
    else 'lvl1. no remarks' 
    end remarks,
now() checktime 
from 
(
	select keydev_prod.device, cast(keydev_prod.target_amount as unsigned) as target_amount , cast(keydev_prod.finished_amount as unsigned) as finished_amount	
    , cast(keydev_prod.shortage as unsigned) as shortage,
		ifnull(sum(sol.quantity),0) sol_qty,  percent_error 
    from 
	(
		SELECT device, target_amount, finished_amount, shortage, ((abs(shortage)/target_amount)*100) percent_error  
		from keydevice_production where ((abs(shortage)/target_amount)*100) > 10 
		group by device
	) keydev_prod
	left join 
	(
		select * from solution
	) sol on keydev_prod.device = sol.device_name
	group by keydev_prod.device
) keyfil
left join
(
	select * from wip
) wip on keyfil.device = wip.device
group by keyfil.device;


having sol_qty < wip_qty;

select *, case when cast(target_amount as unsigned)>0 then 'val' end as test from keydevice_production