import cgi
import os

try:
    # Windows needs stdio set for binary mode.
    import msvcrt
    msvcrt.setmode(0, os.O_BINARY)  # stdin  = 0
    msvcrt.setmode(1, os.O_BINARY)  # stdout = 1
except ImportError:
    pass

form = cgi.FieldStorage()

# A nested FieldStorage instance holds the file
fileitem = form['file']

# Test if the file was uploaded
if fileitem.filename:

    # Specify the target directory for saving uploaded files
    upload_directory = os.getenv('UPLOAD_DIRECTORY')

    # Ensure the target directory exists; create it if necessary
    if not os.path.exists(upload_directory):
        os.makedirs(upload_directory)

    # Strip leading path from the file name to avoid directory traversal attacks
    fn = os.path.basename(fileitem.filename)
    target_path = os.path.join(upload_directory, fn)

    # Save the file to the specified directory
    with open(target_path, 'wb') as target_file:
        target_file.write(fileitem.file.read())

    message = 'The file <b>{}</b> was uploaded successfully to <b>{}</b>'.format(fn, upload_directory)

else:
    message = 'No file was uploaded'

html_response = '<html><h1>this is python upload test</h1><p> "{}" </p></html>'.format(message)

print(html_response)



