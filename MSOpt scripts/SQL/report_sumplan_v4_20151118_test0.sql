
	select sp.MI, sp.MF, sp.PKG_SIZE, sp.LB, sp.TF, sp.NROWS, sp.SUM, sp.RNUM, r.SUM_PPH, pin
	from
	(
	select MI, MF, PKG_SIZE, LB, TF, NROWS, SUM,
		case when MI = @prev_MI then (@rownum := @rownum + 1) else @rownum := 1 end as RNUM, pin,
		@prev_MI := MI
	from
	(
		select s.Machine_Instance as MI, s.Machine_Family_Name as MF, s.Package as PKG_SIZE, dw.atss_loadboard as LB, s.Tooling_Family_Name as TF,
			count(*) as NROWS, sum(s.Quantity) as SUM, pin
		from 
        (select s.Machine_Instance, s.Machine_Family_Name, s.Package, s.Tooling_Family_Name, s.Quantity,s.device_name, pin
        from solution s inner join wip on wip.device = s.device_name and s.lot_name = wip.lot_name) s
		left join
		(select * from smsdw) dw on s.device_name =  dw.device
		group by s.Machine_Instance, s.Machine_Family_Name, s.Package, pin, s.Tooling_Family_Name
		order by s.Machine_Instance, s.Package
	) as a, (select @prev_MI := null) i
	 join (select @rownum := 0) r
	) sp
	
	left join    
	
	(
	select r.package,r.Machine_Family,r.Tooling_Family,sum(r.pph) as SUM_PPH from 
	route r
	group by r.package,r.Machine_Family,r.Tooling_Family
	) r on r.Package = sp.PKG_SIZE and r.Machine_Family = sp.MF and r.Tooling_Family = sp.TF

;