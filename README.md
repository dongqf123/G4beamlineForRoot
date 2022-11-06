# G4beamlineForRoot
# chain_count.cpp:
    利用TChain读入多个root文件，并根据筛选条件计算不同筛选条件下的粒子数占比，用来计算不同发射度或束斑孔径下的粒子数占比

# chain_draw.cpp：
    利用TChain读入多个root文件，并只画六个G4bl探测器探测到的粒子的相空间等
    
# chain_drawTotal：
    利用TChain读入多个root文件，并只画Det#探测器探测到的粒子的相空间等
    
# get_data_from_chain：
   利用TChain读入多个root文件，并将其的每一个探测器转为一个txt文件
