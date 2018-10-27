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



def get_measurements_values(cell_range, service, dimension="ROWS"):
    result = service.spreadsheets().values().get(
            spreadsheetId=SHEETS["Measurements"],
            range=cell_range,majorDimension=dimension).execute()
    return result.get('values', [])




def get_named_header(search_str, sheet_name,  service):
    print "Looking for header with name %s in %s" % (search_str, sheet_name)
    batch_size = 100
    batches = 10
    total_rows = batch_size * batches
    for r,x in [("%s!A%d:A%d" % (sheet_name, x+1, x+batch_size) , x) for x in xrange(0,total_rows,batch_size) ]:
        values = get_measurements_values(r,service, "COLUMNS")
        if values:
            for i, v in enumerate(values[0]):
                if v == search_str:
                    row = (x+i+1)
                    cell_range = "%s!A%d:B%d"  % (sheet_name,row,row)
                    header_values = get_measurements_values(cell_range,service)[0]
                    header_values.append(row)
                    return tuple(header_values)


def get_resource_by_name(name, resource):
    service = _get_service()
    header = get_named_header(name, resource, service)
    print "In get_resource_by_name - "
    if header:
        name, desc, row = header
        print "name, desc %s %s" % (name, desc)
        cell_range = "%s!A%d:Z%d"  % (resource, row+1,row+64)
        data =  get_measurements_values(cell_range,service)
        new_data = []
        for  entry in data:
            if len(entry) == 0:
                break
            new_data.append(entry)
        return tuple([name, desc, new_data])





def get_raw_board_data():
    service = _get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=SHEETS["Measurements"],
        range='Board!A2:D5').execute()
    values = result.get('values', [])
    print(values)
    offset_result = service.spreadsheets().values().get(
        spreadsheetId=SHEETS["Measurements"],
        range='Board!E6').execute()
    offset_values = offset_result.get('values')

    offset = offset_values[0][0] if offset_values else 0


    ground_result = service.spreadsheets().values().get(
        spreadsheetId=SHEETS["Measurements"],
        range='Board!A1').execute()
    ground = ground_result.get('values')[0][0]

    offset = offset_values[0][0] if offset_values else 0



    return (values, offset, ground)
