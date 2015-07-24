import QtQuick 2.5
import QtQuick.Window 2.2
import GameUpQt 1.0

Window {
    visible: true

    property bool gameServerAlive: false
    property Gamer gamer: null
    property Leaderboard leaderboard: null
    property string user: ""
    property string userToken: ""

    onGameServerAliveChanged: {
        if (gameServerAlive) {
            userToken = gameup.loginAnonymous(user)
            if (userToken !== "") {
                gameup.addUserToken(user, userToken) //fillup table to associate user with its GameUp token
                gamer = gameup.getGamer(user) //get Gamer istance for given username
                console.log("gamer:", gamer.name, gamer.nickname, gamer.createdAt)
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
        apiKey: "<api key here>"
        leaderboardID: "<leaderboard id here>"
        Component.onCompleted: {
            gameServerAlive = ping() //ping server make sure it works
        }
    }
}

