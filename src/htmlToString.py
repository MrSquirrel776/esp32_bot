htmlPath = "app.html"
cppPath = "app.cpp"

html = open(htmlPath)
htmlContent = html.read()

cpp = open(cppPath, "w")
cpp.write(f'char* GetHtml() {{return R"rawliteral({htmlContent})rawliteral";}}')

print(f"Succesfully updated '{cppPath}' with content of '{htmlPath}'.")