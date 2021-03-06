#!/bin/bash -xe


cd $(dirname $0)

function test()
{
  ../examples/apply_color_dong2010 $1 $2 
  ../examples/apply_color_reinhard2001 $1 $2 &> color.log
  dir="reinhard2001"
  mv dong.jpg "$dir/$1-$2-result.jpg"
  mv apply-color-reinhard.jpg "$dir/$1-$2-result-rein.jpg"
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
    <img src="$dir/$1-$2-result.jpg" width="256px" height="256px" />
  </td>
  <td>
    <img src="$dir/$1-$2-result-rein.jpg" width="256px" height="256px" />
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
<th>Result (Reinhard et al, 2001)</th>
</tr>
EOH

test "storm.jpg" "source.jpg"
test "forest.jpeg" "les_modry.jpg"
test "nemcsics.jpg" "r-b-kitaj-ronald-b-kitaj-the-oak-tree-1991.jpg"

#test "storm.jpg" "alpy.JPG"
#test "storm.jpg" "hrad.JPG"

#test "les_modry.jpg" "les_zeleny.jpg"
#test "forest.jpeg" "les_zeleny.jpg"
#test "fantasy.jpg" "les_zeleny.jpg"
#test "darkforest.jpg" "les_zeleny.jpg"


#test "les_modry.jpg" "dest.jpg"
test "forest.jpeg" "dest.jpg"
#test "fantasy.jpg" "dest.jpg"
#test "darkforest.jpg" "dest.jpg"

#test "fantasy_stred.jpg" "les_mlha.jpg"
#test "les_modry.jpg" "les_mlha.jpg"
#test "forest.jpeg" "les_mlha.jpg"
#test "fantasy.jpg" "les_mlha.jpg"
#test "darkforest.jpg" "les_mlha.jpg"

#test "les_modry.jpg" "viki.jpg"
#test "forest.jpeg" "viki.jpg"
#test "fantasy.jpg" "viki.jpg"
#test "fantasy_stred.jpg" "viki.jpg"
#test "darkforest.jpg" "viki.jpg"


test "sunrise-sea.jpg" "blue-sea2.jpg" 



cat >>report.html <<EOF
</table>
</body>
</html>
EOF
