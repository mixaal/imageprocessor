#!/bin/bash -xe


function test()
{
  ../examples/apply_color_reinhard2001 $1 $2
  dir="reinhard2001"
  mv apply-color-reinhard.jpg "$dir/$1-$2-result.jpg"
  mv histogram-matched.jpg "$dir/$1-histogram.jpg"
  mv histogram-original.jpg "$dir/$2-histogram.jpg"
  mv histogram-result.jpg "$dir/$1-$2-histogram.jpg"
  cat >>report.html <<ROW  
<tr>
   <td>
     <img src="$1" width="256px" height="256px" />
  </td>
  <td>
     <img src="$2" width="256px" height="256px" />
  </td>
  <td>
    <img src="$dir/$1-$2-result.jpg" width="256px" height="256px" />
  </td>
  <td>
    <img src="$dir/$1-histogram.jpg" width="256px" height="256px" />
  </td>
  <td>
    <img src="$dir/$2-histogram.jpg" width="256px" height="256px" /> 
  </td>
  <td>
    <img src="$dir/$1-$2-histogram.jpg" width="256px" height="256px" />
  </td>
</tr>
ROW
}

cat >report.html <<EOH
<html>
<body>
<table>
<tr>
<th>Take tonality from</th>
<th>Apply tonality to</th>
<th>Result</th>
<th>Tonality Source Histogram</th>
<th>Tonality (original) Destination Histogram</th>
<th>Resulting Histogram</th>
</tr>
EOH

test "les_modry.jpg" "les_zeleny.jpg"
test "forest.jpeg" "les_zeleny.jpg"
test "bosque.jpg" "les_zeleny.jpg"


test "les_modry.jpg" "dest.jpg"
test "forest.jpeg" "dest.jpg"
test "bosque.jpg" "dest.jpg"

test "sunrise-sea.jpg" "blue-sea2.jpg" 

cat >>report.html <<EOF
</table>
</body>
</html>
EOF
