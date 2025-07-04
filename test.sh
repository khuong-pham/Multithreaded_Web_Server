#!/bin/bash

echo "🔍 Debug: Testing file serving..."

# Check if files exist
echo "📁 Checking file structure:"
if [ -f "public/index.html" ]; then
    echo "✅ public/index.html exists"
else
    echo "❌ public/index.html missing"
fi

if [ -f "public/css/style.css" ]; then
    echo "✅ public/css/style.css exists"
else
    echo "❌ public/css/style.css missing"
fi

if [ -f "public/js/script.js" ]; then
    echo "✅ public/js/script.js exists"
else
    echo "❌ public/js/script.js missing"
fi

echo ""
echo "🏗️ Building and testing..."

# Build
./build.sh
cd build

echo ""
echo "📂 Working directory: $(pwd)"
echo "📁 Contents:"
ls -la

echo ""
echo "📁 Checking for public directory:"
if [ -d "public" ]; then
    echo "✅ public/ directory exists"
    ls -la public/
    if [ -f "public/index.html" ]; then
        echo "✅ public/index.html exists in build"
    else
        echo "❌ public/index.html missing in build"
    fi
else
    echo "❌ public/ directory missing in build"
fi

echo ""
echo "🚀 Starting server (run in background)..."
./webserver &
SERVER_PID=$!

# Wait for server to start
sleep 2

echo ""
echo "🧪 Testing endpoints..."

# Test home page
echo "Testing http://localhost:8080/"
curl -I http://localhost:8080/ 2>/dev/null | head -5

echo ""
echo "Testing http://localhost:8080/css/style.css"
curl -I http://localhost:8080/css/style.css 2>/dev/null | head -5

echo ""
echo "Testing http://localhost:8080/js/script.js"  
curl -I http://localhost:8080/js/script.js 2>/dev/null | head -5

# Stop server
kill $SERVER_PID 2>/dev/null

echo ""
echo "✅ Test complete!"