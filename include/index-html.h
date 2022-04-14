    const char *index_html =
        "<head>\
            <style>\
                body{background-color:black; color:yellow;font-family:sans-serif;font-size:12}\
                .status{margin-left:30vw;padding-left:1vw;background-color:darkslateblue; color:white;width:30vw;font-size:24}\
                .control{margin-left:30vw;padding-left:1vw;vertical-align:center;background-color:darkslateblue; color:white;width:30vw;height:10vh;font-size:24}\
                .buttonStyle{height:75px;width:100px;font-size:24}\
            </style>\
            <script>\
                setInterval(function()\
                {\
                    getData();\
                }, 5000); \
                function getData() {\
                  var xhttp = new XMLHttpRequest();\
                  xhttp.onreadystatechange = function() {\
                    if (this.readyState == 4 && this.status == 200) {\
                      document.getElementById('volts').innerHTML =\
                      this.responseText;\
                    }\
                  };\
                  xhttp.open('GET', 'read', true);\
                  xhttp.send();\
                }\
            </script>\
         </head>\
         <body>\
             <div id='volts' class='status'>%.2f</div>\
             <div class='control'>&nbsp</div>\
             <div class='control'>\
                <form action='/update' method='GET'>\
                    <span> \
                    <input type = 'submit' id = 'button1' name = 'button1' value= 'F1' class='buttonStyle'>\
                    <input type = 'submit' id = 'button2' name='button2' value= 'F2' class='buttonStyle'>\
                    <input type = 'submit' id = 'button3' name='button3' value= 'F3' class='buttonStyle'>\
                    <input type = 'submit' id = 'button4' name='button4' value= 'F4' class='buttonStyle'>\
                    </span>\
                </form>\
            </div>\
         </body>";
