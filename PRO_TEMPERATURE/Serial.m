function varargout = shoudongshuju11_10(varargin)
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @shoudongshuju11_10_OpeningFcn, ...
                   'gui_OutputFcn',  @shoudongshuju11_10_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before shoudongshuju11_10 is made visible.
function shoudongshuju11_10_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to shoudongshuju11_10 (see VARARGIN)

% Choose default command line output for shoudongshuju11_10
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes shoudongshuju11_10 wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = shoudongshuju11_10_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in openchuankou.
function openchuankou_Callback(hObject, eventdata, handles)
% hObject    handle to openchuankou (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
delete(instrfindall);
global zuobiao
global result
delta = 0;
s1 = serial('COM3','Parity','none','BaudRate',115200,'DataBits',8,'StopBits',1,'InputBufferSize',500);%定义串口


h=1;
for i=1:100
k=1;
fopen(s1);
out = fread(s1,500);%读取的串口数据
fclose(s1);
fid = fopen('shuju.bin','w');
fwrite(fid,out,'uint8');
fclose(fid);

fid  = fopen('shuju.bin','rb');   
A = fread(fid,500);
fclose(fid);



%找到正确数据的位置
m=1;
while(A(m)~=0||A(m+1)~=0||A(m+3)~=0||A(m+5)~=0||A(m+7)~=0) 
    m = m+1;
    if(m+7>500)  
        break;
    end
end

%把每个轴的转化为十进制
k = 1;
while((length(A)-m)>=6)
    zuobiao(k,1) = bitand((A(m+1)*16+A(m+2)),4095);   %取后三位求加速度值，第一位为0，F表示方向，bitand是与操作，直接和0xFFF（4095）与操作，直接得出值
    zuobiao(k,2) = bitand((A(m+3)*16+A(m+4)),4095);
    zuobiao(k,3) = bitand((A(m+5)*16+A(m+6)),4095);
    m = m+7;
    k = k+1;
end

%计算每个点的合加速度
for l = 1:length(zuobiao(:,1))
    result(l) =  zuobiao(l,1)^2 +zuobiao(l,2)^2+ zuobiao(l,3)^2;
end

% %计算不符合要求的数据点
% % B = find(delta<33.0150|delta>514.6777);
%符合要求的数据点的个数
 result_old = delta;
delta = length(result) - length(find(result< 5.3925e+04|result>3.2494e+07));  
axes(handles.plot);
a = [h,h+1];
b = [result_old,delta];
plot(a,b);
hold on;
set(handles.edit1,'string',['程序进行中：',num2str(h),'%']);
h = h+1;
end


% --- Executes on button press in close.
function close_Callback(hObject, eventdata, handles)
% hObject    handle to close (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close;

% --- Executes during object creation, after setting all properties.
function plot_CreateFcn(hObject, eventdata, handles)
% hObject    handle to plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: place code in OpeningFcn to populate plot


% --- Executes on button press in clear_plot.
function clear_plot_Callback(hObject, eventdata, handles)
% hObject    handle to clear_plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
axes(handles.plot);
hg = get(gca,'children');
delete(hg);



function edit1_Callback(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of edit1 as text
%        str2double(get(hObject,'String')) returns contents of edit1 as a double


% --- Executes during object creation, after setting all properties.
function edit1_CreateFcn(hObject, eventdata, handles)
% hObject    handle to edit1 (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end