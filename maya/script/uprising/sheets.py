from apiclient.discovery import build
from httplib2 import Http
from oauth2client import file, client, tools

SHEETS = {"Measurements": '1ZwXuS9Yb2YTZ9NRldzn1ecA5dtihZaZtXPo7VAYCl1s'}
SCOPES = 'https://www.googleapis.com/auth/spreadsheets.readonly'
STORE = file.Storage('/Users/julian/dev/w/uprising/token.json')
CREDENTIALS = '/Users/julian/dev/w/uprising/credentials.json'


def _get_service():
    creds = STORE.get()
    if not creds or creds.invalid:
        flow = client.flow_from_clientsecrets(CREDENTIALS, SCOPES)
        creds = tools.run_flow(flow, STORE)
    service = build('sheets', 'v4', http=creds.authorize(Http()))
    return service


# def get_raw_brushes_data():
#     service = _get_service()
#     result = service.spreadsheets().values().get(
#         spreadsheetId=SHEETS["Measurements"],
#         range='Brushes!A2:J18').execute()
#     values = result.get('values', [])
#     return values


def get_raw_board_data():
    service = _get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=SHEETS["Measurements"],
        range='Board!A2:D5').execute()
    values = result.get('values', [])

    offset_result = service.spreadsheets().values().get(
        spreadsheetId=SHEETS["Measurements"],
        range='Board!E6').execute()
    offset_values = offset_result.get('values')
    print offset_values
    offset = offset_values[0][0] if offset_values else 0

    return (values, offset)
