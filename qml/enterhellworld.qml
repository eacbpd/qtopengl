import QtQuick 2.0  

Rectangle {  
width: 200  
			   height: 100  
			   color: "red"  

			   Text {  
					   anchors.centerIn: parent  
							   text: "Hello, World!"  
			   }  

focus: true  
			   Keys.onPressed: {  
					   if (event.key == Qt.Key_Return) {  
							   color = "blue";  
							   event.accepted = true;  
					   }  
			   }  
}  
