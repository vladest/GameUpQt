import QtQuick 2.5
import QtQuick.Window 2.2
import GameUpQt 1.0
import QtWebView 1.1

Window {
    visible: true

    property bool gameServerAlive: false
    property Gamer gamer: null
    property Leaderboard leaderboard: null
    property string user: ""
    property string userToken: ""

    onGameServerAliveChanged: {
        if (gameServerAlive) {
            webViewID.visible = true
            userToken = gameup.login(GameUpQt.GameUp, user)
            webViewID.visible = false
            if (userToken !== "") {
                gameup.addUserToken(user, userToken) //fillup table to associate user with its GameUp token
                gameup.updateGamerAchievments(user)
                gamer = gameup.getGamer(user) //get Gamer istance for given username
                console.log("gamer:", gamer.name, gamer.nickname, gamer.createdAt)
                gameup.updateGamerLeaderboard(user)
                leaderboard = gameup.getLeaderboard(user) //get leaderboard for given game
                console.log("leaderboard name", leaderboard.name)
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Text {
        text: qsTr("Hello World")
        anchors.centerIn: parent
    }

    GameUpQt {
        id: gameup
        apiKey: "18d8572ef7a947058c8bc03f5c7c9376" // replace with your apiKey here
        leaderboardID: "e05fbaee728644ce89e943ad5c5db6f2" //replace with your leaderboard id here
        webView: webViewID
        Component.onCompleted: {
            gameServerAlive = ping()
        }
    }

    WebView {
        id: webViewID
        anchors.fill: parent
        visible: false
        z: 100
    }

}

