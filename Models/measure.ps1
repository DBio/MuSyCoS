$sizes = (8,16,32,64,128,256,512)
$degrees = (4)
$dims = (4)
foreach($size in $sizes) {
	foreach($degree in $degrees) {
		foreach($dim in $dims) {
			..\Release\Generator.exe $size $degree $dim;
			$name = "model_" + $size + "_" + $degree + "_" + $dim + ".pasn";
			#echo $name;
			$Time = (Measure-Command{ ..\Release\MuSyCoS.exe $name -s }).TotalSeconds;
			$output = "model_" + $size + "_" + $degree + "_" + $dim + "_stable.csv";
			$LineC = (Import-Csv $output | Measure-Object).count;
			$result = " " + $size + " " + $degree + " " + $dim + " " + $Time + " " + $LineC;
			echo $result;
		}
	}
}

# ..\Release\MuSyCoS.exe .\model_10_2_2.pasn -s 
