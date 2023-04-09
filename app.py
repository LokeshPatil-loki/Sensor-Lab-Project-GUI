import streamlit as st
from pysondb import db
import serial
import ast

# Define shared list to store commands
commands = []
ser = serial.Serial('/dev/ttyACM0', 9600) 

def readIRSignal():
    count = 0
    irDataList = []
    while count < 6:
        # Read data from Arduino
        data = ser.readline().decode().strip()
        # data = ast.literal_eval(data)
            # Print data to console
        irDataList.append(data)
        print(data)
        count += 1
    decodedSignal = max(irDataList,key=irDataList.count)
    # decodedSignal = ast.literal_eval(decodedSignal)
    return decodedSignal

# Program Page
def program_page():
    myDb = db.getDb('db.json')
    st.title('Program')
    
    # Select or create group
    groupsList = set([item["group"] for item in myDb.getAll()])
    group = st.selectbox('Select or create group', ["Create New Group"] + list(groupsList))
    if group == 'Create New Group':
        group = st.text_input('Enter group name')
    
    # Name of command
    command_name = st.text_input('Enter name of command')
    
    # # Command
    # command = st.text_input('Enter command')

    # Recieve Command
    global command
    command = []
    btn_recieve_command = st.button("Recieve Command")
    if btn_recieve_command:
        info = st.info("Recieving Command, Keep pressing button in small intervals until output is shown")
        st.session_state.command = readIRSignal()
        st.write(st.session_state.command)
        del info
        
    # Store command
    if st.button('Submit'):
        commands.append({'group': group, 'name': command_name, 'command': command})
        data = {
            "group": group,
            "name": command_name,
            "command": st.session_state.command,

        }
        myDb.add(data)
        groupsList = set([item["group"] for item in myDb.getAll()])
        # Clear inputs
        # st.write(st.session_state.command)
        st.session_state.command = ''
        st.session_state.command_name = ''
        st.success('Command stored successfully')
        # st.write(myDb.getAll())
    ser.write("read".encode())


# Controll Page
def control_page():
    
    myDb = db.getDb('db.json')
    st.title('Control')
    groupsList = set([item["group"] for item in myDb.getAll()])
    group = st.selectbox("Select Group",list(groupsList))
    # Display commands
    st.subheader("Available Commands")
    if group:
        commandsByGroup = myDb.getByQuery({"group": group})
        columns = st.columns(2)
        st.markdown("""
        <style>
        div.stButton > button:first-child {
            padding: 50px;
            width: 100%;
        }
        </style>""", unsafe_allow_html=True)


        for i in range(0,len(commandsByGroup)):
            column_index = i % 2
            with columns[column_index]:
                btn = st.button(commandsByGroup[i]["name"])
            if btn:
                st.write(commandsByGroup[i]["command"].encode())
                ser.write(commandsByGroup[i]["command"].encode())
    
    ser.write("send".encode())

# Define app
def app():
    st.set_page_config(page_title='Command App')
    
    # Define pages
    pages = {
        'Program': program_page,
        'Control': control_page
    }
    
    # Sidebar navigation
    page = st.sidebar.selectbox('Select page', list(pages.keys()))
    
    # Display selected page
    pages[page]()
    
# Run app
if __name__ == '__main__':
    app()
