var dialogWindow

function createObject(src, parent, list) {
    list = typeof list == 'undefined'?{}:list
    print("Create Component for " + src)

    var component = Qt.createComponent(src);
    var object
    if (component.status == Component.Ready) {
        object = component.createObject(parent, list);
        if (object == null) {
            // Error Handling
            console.log("Error creating object");
        }
        else {
            return object
        }
    }
    else if (component.status == Component.Error) {
        print("Error loading component:", component.errorString());
    }
    return null
}

function createAndShowWindow(src, parent, force) {
    force = typeof force == 'undefined'?false:force
    print("Create Dialog for " + src)
    var comp = Qt.createComponent(src)
    if(comp.status == Component.Ready){
        var tmpDialogWindow = comp.createObject(parent)
        if (!tmpDialogWindow) {
            // Error Handling
            console.error("Error creating object");
        } else {
            if(dialogWindow) {
                print("Dialog already exists")
                if(!force && dialogWindow.objectName == tmpDialogWindow.objectName) {
                    print("Opening exisiting")
                    dialogWindow.show()
                    dialogWindow.raise()
                    return
                }
                print("Closing existing")
                dialogWindow.close()
            }
            delete dialogWindow
            dialogWindow = tmpDialogWindow
            dialogWindow.show()
        }
    } else if (comp.status == Component.Error) {
        console.error("Error loading component:" + comp.errorString());
    }
}
