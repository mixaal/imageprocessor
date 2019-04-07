#!/bin/bash -xe


cd $(dirname $0)

function test()
{
  ../examples/apply_color_dong2010 $1 $2 $3
  dir="reinhard2001"
  mv dong.jpg "$dir/$1-$2-result-$3.jpg"
#  mv histogram-matched.jpg "$dir/$1-histogram.jpg"
#  mv histogram-original.jpg "$dir/$2-histogram.jpg"
#  mv histogram-result.jpg "$dir/$1-$2-histogram.jpg"
  cat >>report.html <<ROW  
<tr>
   <td>
     <img src="$1" width="256px" height="256px" />
  </td>
  <td>
     <img src="$2" width="256px" height="256px" /> 
  </td>
  <td>
    <img src="$dir/$1-$2-result-$3.jpg" width="256px" height="256px" />
    <p>Number of dominant colors: $3</p>
  </td>
<!--
  <td>
    <img src="$dir/$1-histogram.jpg" width="256px" height="256px" />
  </td>
  <td>
    <img src="$dir/$2-histogram.jpg" width="256px" height="256px" /> 
  </td>
  <td>
    <img src="$dir/$1-$2-histogram.jpg" width="256px" height="256px" />
  </td>
-->
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
<th>Result (Dong et al, 2010)</th>
</tr>
EOH

test "nemcsics.jpg" "r-b-kitaj-ronald-b-kitaj-the-oak-tree-1991.jpg" 4
test "nemcsics.jpg" "r-b-kitaj-ronald-b-kitaj-the-oak-tree-1991.jpg" 8
test "nemcsics.jpg" "r-b-kitaj-ronald-b-kitaj-the-oak-tree-1991.jpg" 16
test "nemcsics.jpg" "r-b-kitaj-ronald-b-kitaj-the-oak-tree-1991.jpg" 32



cat >>report.html <<EOF
</table>
</body>
</html>
EOF
