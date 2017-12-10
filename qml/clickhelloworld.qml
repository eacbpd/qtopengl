import QtQuick 2.0  
  
Rectangle {  
    width: 200  
    height: 100  
    color: "gray"  
  
    Text {  
        anchors.centerIn: parent  
        text: "Hello, World!"  
    }  
  
    MouseArea {  
        anchors.fill: parent  
        onClicked:{
			{
			parent.color = "red";
			}
		}
    }  
} 
