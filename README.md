# ConvexHullMap
Draws a convex hull of geographic points.

A convex hull is the area enclosed by the smallest convex polygon whose vertices are points from some finite set of points, P and which contains all the points of P. 
This project uses geographic data provided by OpenStreetMap, a non-profit foundation whose aim is to support and enable the development of freely-reusable geospatial data. Specifically, the project uses OpenStreetMap data files in OSM format (an XML format representing Points on Earth). 

If you want to download and process OpenStreetMap data for the entire Europe, you will need to download the compressed OSM file into your pa-2/data folder and then follow these steps:

make sure you are at pa-2/data
```
cd <PATH>/pa-2/data
```
decompress the downloaded file using bz2
```
bzip2 -d europe-latest.osm.bz2
```
parse the OSM file using the python script and redirect the output to europe-latest.txt
```
python parse-osm.py europe-latest.osm >europe-latest.txt
```

# To compile the project:
make sure you are at pa-2
```
cd <PATH>/pa-2
```
compile the program
```
g++ -std=c++11 search-map.cpp LList.cpp -o search-map
```

# Running the project:
This project finds and prints all the locations in the database that are under a distance radius (in miles) from a given input latitude and longitude. Furthermore, the program only displays results that match a given filter in their description. For example if you have data for the Boston area and want to find all Starbucks under a radius of 10 miles from location (42.37, -71.12) you should type:

make sure you are at pa-2
```
cd <PATH>/pa-2
```
call the program providing all required parameters, you should see an output of 35 locations
```
./search-map 42.37 -71.12 10 starbucks data/boston_massachusetts.txt
```
if you want to understand the meaning of every single parameter call the program with no parameters
```
./search-map
```
The program prints all output to stdout and stderr. If you want to visualize your results you must redirect stdout to data/markers.js: 
1 make sure you are at pa-2
```
cd <PATH>/pa-2
```
call the program providing all required parameters and redirect stdout
```
./search-map 42.37 -71.12 10 starbucks data/boston_massachusetts.txt >data/markers.js
```
After running the program, the output has been saved to data/markers.js, you can then open data/index.html in any browser to visualize your results. 
