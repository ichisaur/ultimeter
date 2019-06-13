"use strict";

var config = {
    authDomain: "ultimeter-altimeter.firebaseapp.com",
    databaseURL: "https://ultimeter-altimeter.firebaseio.com"
};
if (!firebase.apps.length) {
    firebase.initializeApp(config);
}
var database = firebase.database();

database.ref('/location/').on('value', function (snapshot) {
    var curDate = new Date();
    curDate = curDate.setHours(curDate.getHours() - 3);
    console.log(snapshot.val().timestamp);
    console.log(curDate);
    var timeDate = new Date(snapshot.val().timestamp);

    if (timeDate < curDate) {
        document.getElementById('rocket-location-message').innerText = "No Active Tracking Session Found";
    } else {
        document.getElementById('rocket-location-message').innerText = "Your last known position is at: " + snapshot.val().loc;
    }
});