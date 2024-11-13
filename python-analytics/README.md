# IoT-Based Biometric Attendance System with Google Sheets Integration

This project implements an IoT-based biometric attendance system that captures attendance data using fingerprint sensors and stores it in Google Sheets. It provides a Jupyter Notebook (`sheets.ipynb`) to update attendance records, calculate attendance percentages, and highlight defaulters in the Google Sheets document.

## Project Structure

The project is structured as follows:

```
python-analytics/
│
├── sheets.ipynb                # Jupyter notebook to manage attendance and Google Sheets
├── credentials.json            # Google Sheets API credentials (keep it private)
└── README.md                   # This file
```

## Setup Instructions

To run this project, follow the steps below.

### 1. Install Dependencies

Clone this repository to your local machine and navigate to the project directory:

```bash
git clone https://github.com/<your-username>/python-analytics.git
cd python-analytics
```

Then, create a virtual environment and install the required dependencies:

```bash
python -m venv venv
source venv/bin/activate  # On Windows use `venv\Scripts\activate`
pip install -r requirements.txt
```

The `requirements.txt` file should include the following libraries (install them if not already available):

- `google-api-python-client`
- `google-auth`
- `google-auth-httplib2`
- `google-auth-oauthlib`
- `flask`

### 2. Set up Google Sheets API

To interact with Google Sheets, you need to set up the Google Sheets API and authenticate using a service account.

1. Go to the [Google Cloud Console](https://console.cloud.google.com/).
2. Create a new project (or use an existing one).
3. Enable the **Google Sheets API** and **Google Drive API**.
4. Create **Service Account Credentials** and download the `credentials.json` file.
5. Rename the downloaded credentials file to `credentials.json` and place it in the root of the project directory.
6. Share the Google Sheet (you will be interacting with) with the service account’s email address (found in the `credentials.json`).

### 3. Use the Jupyter Notebook

After setting up the credentials, open the `sheets.ipynb` Jupyter notebook to interact with Google Sheets.

- The notebook includes code to connect to Google Sheets, mark attendance based on input times, calculate attendance percentages, and highlight defaulters.

To launch the Jupyter Notebook:

```bash
jupyter notebook sheets.ipynb
```

### 4. Security and Privacy

- **Credentials**: The `credentials.json` file contains sensitive information related to your Google Sheets API access and should not be shared publicly. Always keep it secure.

Ensure that the `credentials.json` is kept private and is not uploaded to GitHub.

### 5. License

This project is licensed under the GNU General Public License (GPL) Version 3, 29 June 2007. See the full license text below:

```
GNU GENERAL PUBLIC LICENSE
   Version 3, 29 June 2007

Copyright (C) 2007 Free Software Foundation, Inc. <https://fsf.org/>
Everyone is permitted to copy and distribute verbatim copies
```
---
```

### Key Points:
- The `credentials.json` is sensitive and should not be uploaded publicly.
- The `NGROK_AUTH_TOKEN` is used for ngrok authentication and should be kept secret.
- The project integrates with Google Sheets to track attendance, calculate percentages, and highlight defaulters.
- Make sure to keep the `.env` file, `credentials.json`, and other sensitive files private using `.gitignore`.
