var sheet_id = "SHEET-ID"; // replace with your Sheet ID
var sheet_name = "Attendance";

function doGet(e) {
  updateEntry(e);
  updateGoogleSheetsWithStudentData();
}

function updateEntry(e) {
  var ss = SpreadsheetApp.openById(sheet_id);
  var sheet = ss.getSheetByName(sheet_name);

  var date = String(e.parameter.date);
  var time = e.parameter.time;
  var sensor = Number(e.parameter.sensor);
  var lastRow = sheet.getLastRow();
  
  var foundRow = -1;
  
  if (sheet.getRange(1, 8).getValue() == "") {
    sheet.getRange(1,8).setValue(date);
  }

  var lastCol = sheet.getLastColumn();

  // Check the last date in the header row to compare
  var uniqueDateFound = false;
  for (var j = 8; j <= lastCol; j++) {
    var existingDate = sheet.getRange(1, j).getValue();
    var formattedExistingDate = (existingDate.getMonth() + 1) + "/" + existingDate.getDate() + "/" + existingDate.getFullYear();
    var newDate = String(date.replace(/^0(\d)\//, "$1/"));
    if (formattedExistingDate == newDate) {
      uniqueDateFound = true; // Date already exists, so don't insert a new column
      break;
    }
  }

  // Check for existing entry
  for (var i = 2; i <= lastRow; i++) {
    var rowFingId = Number(sheet.getRange(i, 1).getValue());
    if (rowFingId == sensor) {
      foundRow = i;
      break;
    }
  }

  // If the date is unique, insert it into the next available column
  if (!uniqueDateFound) {
    sheet.getRange(foundRow, 5).setValue("");
    sheet.getRange(foundRow, 6).setValue("");
    for (var k = 1; k <= lastCol; k++) {
      if (sheet.getRange(1, k + 1).getValue() == "") {
        sheet.getRange(1, k + 1).setValue(date);
        break;
      }
    }
  }
  

  // Insert new entry or update existing one
  if (foundRow === -1) {
    sheet.appendRow([sensor, "", "", "", time, ""]);
  } else {
    var inTime = sheet.getRange(foundRow, 5).getValue();
    if (inTime === "") {
      sheet.getRange(foundRow, 5).setValue(time);
    } else {
      var outTime = sheet.getRange(foundRow, 6).getValue();
      if (outTime === "") {
        sheet.getRange(foundRow, 6).setValue(time); // If out-time is not filled, set it
      } 
    }
  }
  
  return ContentService.createTextOutput("Data processed");
}

function updateGoogleSheetsWithStudentData() {
  var apiUrl = "FIREBASE-URL"; // replace with your Firebase URL 
  var response = UrlFetchApp.fetch(apiUrl);
  var data = JSON.parse(response.getContentText());
  
  var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
  var lastRow = sheet.getLastRow();
  
  for (var i = 2; i <= lastRow; i++) {
    var sheetFingId = sheet.getRange(i, 1).getValue(); // fing-id in column A
    for (var key in data) {
      if (data.hasOwnProperty(key) && data[key].fing_id == sheetFingId) {
        sheet.getRange(i, 2).setValue(data[key].name);          // Update name in column B
        sheet.getRange(i, 3).setValue(data[key].stud_id);       // Update student-id in column C
        sheet.getRange(i, 4).setValue(data[key].stud_email);    // Update student-email in column D
        break; // Exit once matched
      }
    }
  }
  
  Logger.log('Google Sheet updated with student data.');
}
