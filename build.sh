#!/bin/bash

# ChatServer 自动编译脚本 - 修复版
# 使用方法: ./build.sh [debug|release|clean|rebuild]

# 配置变量
PROJECT_DIR="/home/ubuntu/code/ChatServer"
BUILD_DIR="$PROJECT_DIR/build"
BIN_DIR="$PROJECT_DIR/bin"
EXECUTABLE="ChatServer"
CMAKE_FLAGS=""

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查依赖
check_dependencies() {
    local deps=("cmake" "g++" "make")
    local missing=()
    
    for dep in "${deps[@]}"; do
        if ! command -v "$dep" &> /dev/null; then
            missing+=("$dep")
        fi
    done
    
    if [ ${#missing[@]} -ne 0 ]; then
        print_error "缺少以下依赖: ${missing[*]}"
        exit 1
    fi
}

# 创建必要目录
create_directories() {
    if [ ! -d "$BUILD_DIR" ]; then
        print_info "创建构建目录: $BUILD_DIR"
        mkdir -p "$BUILD_DIR"
    fi
    
    if [ ! -d "$BIN_DIR" ]; then
        print_info "创建二进制目录: $BIN_DIR"
        mkdir -p "$BIN_DIR"
    fi
}

# 清理构建
clean_build() {
    print_info "清理构建文件..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"/*
        print_success "构建目录已清理"
    else
        print_warning "构建目录不存在，跳过清理"
    fi
}

# 配置 CMake
configure_cmake() {
    local build_type=$1
    print_info "配置 CMake ($build_type 模式)..."
    
    cd "$BUILD_DIR" || exit 1
    
    case $build_type in
        "debug")
            CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Debug"
            ;;
        "release")
            CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Release"
            ;;
        *)
            CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=RelWithDebInfo"
            ;;
    esac
    
    if cmake .. $CMAKE_FLAGS; then
        print_success "CMake 配置成功"
    else
        print_error "CMake 配置失败"
        exit 1
    fi
}

# 编译项目
compile_project() {
    local num_jobs=$1
    print_info "开始编译 (使用 $num_jobs 个作业)..."
    
    if make -j "$num_jobs"; then
        print_success "编译成功"
    else
        print_error "编译失败"
        exit 1
    fi
}

# 修复：检查可执行文件
check_executable() {
    local executable_path="$BIN_DIR/$EXECUTABLE"
    local build_executable_path="$BUILD_DIR/$EXECUTABLE"
    
    # 首先检查 bin 目录
    if [ -f "$executable_path" ]; then
        print_success "可执行文件已生成: $executable_path"
        
        # 设置执行权限
        chmod +x "$executable_path"
        
        # 显示文件信息
        echo ""
        print_info "文件信息:"
        file "$executable_path"
        echo ""
        print_info "磁盘使用:"
        du -h "$executable_path"
        return 0
    # 然后检查 build 目录
    elif [ -f "$build_executable_path" ]; then
        print_success "可执行文件在构建目录: $build_executable_path"
        
        # 确保 bin 目录存在
        mkdir -p "$BIN_DIR"
        
        # 复制到 bin 目录
        cp "$build_executable_path" "$BIN_DIR/"
        print_success "已复制到: $executable_path"
        
        # 设置执行权限
        chmod +x "$executable_path"
        
        # 显示文件信息
        echo ""
        print_info "文件信息:"
        file "$executable_path"
        echo ""
        print_info "磁盘使用:"
        du -h "$executable_path"
        return 0
    else
        print_error "可执行文件未生成在预期位置"
        print_info "检查了以下位置:"
        print_info "  - $executable_path"
        print_info "  - $build_executable_path"
        
        # 尝试查找实际生成位置
        local found_files=$(find "$BUILD_DIR" -name "$EXECUTABLE" -type f 2>/dev/null)
        if [ -n "$found_files" ]; then
            print_info "在其他位置找到可执行文件:"
            echo "$found_files"
            
            # 复制第一个找到的文件到 bin 目录
            local first_file=$(echo "$found_files" | head -n1)
            mkdir -p "$BIN_DIR"
            cp "$first_file" "$executable_path"
            chmod +x "$executable_path"
            print_success "已复制到: $executable_path"
            return 0
        fi
        return 1
    fi
}

# 主函数
main() {
    local mode=${1:-"debug"}
    local num_jobs=$(nproc)  # 使用 CPU 核心数作为并行作业数
    
    print_info "开始 ChatServer 自动编译"
    print_info "模式: $mode"
    print_info "项目目录: $PROJECT_DIR"
    print_info "并行作业数: $num_jobs"
    echo "========================================"
    
    case $mode in
        "debug"|"release")
            check_dependencies
            create_directories
            configure_cmake "$mode"
            compile_project "$num_jobs"
            check_executable
            ;;
        "clean")
            clean_build
            ;;
        "rebuild")
            check_dependencies
            clean_build
            create_directories
            configure_cmake "debug"
            compile_project "$num_jobs"
            check_executable
            ;;
        "help"|"-h"|"--help")
            echo "使用方法: $0 [debug|release|clean|rebuild|help]"
            exit 0
            ;;
        *)
            print_error "未知模式: $mode"
            echo "使用方法: $0 [debug|release|clean|rebuild|help]"
            exit 1
            ;;
    esac
    
    echo "========================================"
    print_success "编译完成!"
    
    # 显示运行示例
    if [[ "$mode" =~ ^(debug|release|rebuild)$ ]]; then
        echo ""
        print_info "运行示例:"
        echo "  $BIN_DIR/$EXECUTABLE 127.0.0.1 6000"
    fi
}

# 运行主函数
main "$@"
