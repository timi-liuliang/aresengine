for i in selection do
(
	if i.material.opacityMap != undefined then
	(
		i.material.opacityMap.filename=replace i.material.opacityMap.filename (i.material.opacityMap.filename.count-2) 3 "dds"
	)
)

for i in selection do
(
	if i.material.opacityMap != undefined then
	(
	 	i.material.diffuseMap.filename=replace i.material.diffuseMap.filename (i.material.diffuseMap.filename.count-2) 3 "dds"
	)
)