const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
        body {
            height: 100vh;
            width: auto;
            margin: auto;
            text-align: center;
            display: flex;
            flex-direction: column;
            font-family: Rockwell;
        }

        .content {
            background-image: url('https://www.matematiksel.org/wp-content/uploads/2017/03/deprem.jpg?ezimgfmt=rs%3Adevice%2Frscb220-1');
            height: 100vh;
            color: white;
            flex: 1;
            background-size: cover;
            overflow: auto;
        }

        table {
            width: 100%;
            font-size: 24px;
            margin: 10px 20px;
        }

        td,
        th {
            text-align: center;
            padding: 4px 0;
        }

        p {
            font-size: 18px;
            padding-top: 10px;
        }

        .readings-container {
            background: rgba(65, 65, 65, 0.7);
            border-radius: 25px;
            margin: 0 10px;
        }

        .chart-container {
            width: auto;
            height: 200px;
        }

        .chart-container p {
            margin: 5px 0;
        }

        canvas {
            border: black 1px solid;
            background-color: white;
            margin-top: 10px;
            width: auto;
        }
    </style>
</head>

<body>

    <div class="header">
        <h1>THE USE OF INTERNET OF THINGS (IOT) BASED ACCELEROMETER SENSOR <BR> AS STRUCTURAL HEALTH MONITORING DEVICE
        </h1>
    </div>

    <div class="content">

        <div class="address">
            <h2> JMJ MARIST BROTHERS
                <br> NOTRE DAME OF MARBEL UNIVERSITY
                <br> CITY OF KORONADAL, SOUTH COTABATO
            </h2>
        </div>

        <div id="textData" style="width:100%; float:left">

            <h2>Acceleration of Structure</h2>
            <h3 style="font-style: italic;"> NOTE: 1g = 9.8 m/s²</h3>

            <div class="readings-container" id="currentRead">
                <P>Current Displacement</P>
                <table id="currentReadings">
                    <tr>
                        <th> X (g) </th>
                        <th> Y (g) </th>
                        <th> Z (g) </th>
                    </tr>
                    <tr>
                        <td id="cX">0</td>
                        <td id="cY">0</td>
                        <td id="cZ">0</td>
                    </tr>
                </table>
            </div>

            <div class="readings-container" id="highestRead">
                <P>Highest Displacement</P>
                <table id="highestReadings">
                    <tr>
                        <th> X (g) </th>
                        <th> Y (g) </th>
                        <th> Z (g) </th>
                    </tr>
                    <tr>
                        <td id="hX">0</td>
                        <td id="hY">0</td>
                        <td id="hZ">0</td>
                    </tr>
                </table>
            </div>

            <div class="readings-container" id="statusRead">
                <P>Displacement Status</P>
                <table id="highestReadings" style="height:200px">
                    <tr>
                        <th>X</th>
                        <th>Y</th>
                        <th>Z</th>
                    </tr>
                    <tr>
                        <td id="sX">Normal</td>
                        <td id="sY">Moderate</td>
                        <td id="sZ">Dangerous</td>
                    </tr>
                </table>
            </div>



        </div>
    </div>

    <script>

        //Update data every 0.25 seconds 
        setInterval(function () {
            getData();
        }, 250); //250mSeconds update rate

        function getData() {
            var xhttp = new XMLHttpRequest();
            xhttp.onreadystatechange = function () {
                if (this.readyState == 4 && this.status == 200) {
                    //Push the data in array
                    var txt = this.responseText;
                    var obj = JSON.parse(txt);

                    var xValue = obj.xVal;
                    var yValue = obj.yVal;
                    var zValue = obj.zVal;

                    updateReadings("cX", "hX", xValue);
                    updateReadings("cY", "hY", yValue);
                    updateReadings("cZ", "hZ", zValue);
                }
            };
            xhttp.open("GET", "readADC", true); //Handle readADC server on ESP8266
            xhttp.send();
        }


        //update the current readings and the highest
        function updateReadings(currentId, highestId, value) {
            var currentPoint = document.getElementById(currentId);
            var highestPt = document.getElementById(highestId);

            currentPoint.innerHTML = value;

            if (Math.abs(Number(value)) >= Math.abs(Number(highestPt.innerHTML))) {
                highestPt.innerHTML = value;
            }
        }

    </script>

</body>

</html>
)=====";
