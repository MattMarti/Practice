
function pow3(x) {
    return x * x * x;
}
    
while (true) {
    var yourName = prompt("What is your name?");
    if (yourName != null && yourName != "") {
document.getElementById("sayHello").innerHTML = "Hello \"" + yourName + "\"";
break;
    } else {
alert("Please enter a name");
    }
}

document.write("5 + 4 = ", 5+4, "<br />");
document.write("5 + 4.2 = ", 5+parseFloat("4.2"), "<br />");

var ans_length = 5;
var ansArray = [ans_length]
for (j = 0; j <= ans_length; j++) {
    ansArray[j] = j+parseFloat("4.2");
    document.write(j, " + 4.2 = ", ansArray[j], "<br />");
}

document.write("pow3(3) = ", pow3(3), "<br />");
