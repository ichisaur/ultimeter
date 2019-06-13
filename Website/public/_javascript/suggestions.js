
var userID = document.cookie.replace(/(?:(?:^|.*;\s*)username\s*\=\s*([^;]*).*$)|^.*$/, "$1");
var user_name = document.cookie.replace(/(?:(?:^|.*;\s*)name\s*\=\s*([^;]*).*$)|^.*$/, "$1");


var config = {
    authDomain: "northwestern-task.firebaseapp.com",
    databaseURL: "https://northwestern-task.firebaseio.com"
}


if (!firebase.apps.length) {
    firebase.initializeApp(config);
}
var database=firebase.database();
var taskListURL= '/taskList/' + userID + "/";


database.ref(taskListURL).once('value', function(snapshot) {

    var taskJSON = snapshot.val();
    var compDate = null;
    var lowestEvent = null;
    var key = null;
    for (key in taskJSON) {
        if (compDate==null) compDate= new Date(taskJSON[key]["dueDateValue"]);

        if (!taskJSON[key]["isDone"]) {
            var tempDate = new Date(taskJSON[key]["dueDateValue"]);
            if (compDate >= tempDate) {
                compDate = tempDate;
                lowestEvent = key;
            }
        }
    }

    if (key == null) {
        document.getElementById('suggestion-box').innerText = "You're all caught up!";

    }
    document.getElementById('suggestion-box').innerHTML = 'Your suggested task is: <strong>' +taskJSON[lowestEvent]['taskNameValue'] + '. </strong><br><br> Check it off in Task View when you are done!';
});


document.addEventListener('DOMContentLoaded', () => {
    var user_name = document.cookie.replace(/(?:(?:^|.*;\s*)name\s*\=\s*([^;]*).*$)|^.*$/, "$1")
    var user_id = document.cookie.replace(/(?:(?:^|.*;\s*)username\s*\=\s*([^;]*).*$)|^.*$/, "$1")
    if (user_name == "" || user_id == "") {
        alert("You have been logged out.");
        document.location.href='index.html';

    }
  });
