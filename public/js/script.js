// JavaScript for Multithreaded Web Server Demo
console.log('🚀 Multithreaded Web Server JavaScript loaded successfully!');

document.addEventListener('DOMContentLoaded', function() {
    console.log('DOM loaded, initializing features...');
    
    // Add some interactive features
    initializeFeatures();
    addPageLoadInfo();
    addNavigationEffects();
    showServerInfo();
});

function initializeFeatures() {
    // Add click effects to feature cards
    const features = document.querySelectorAll('.feature');
    features.forEach(feature => {
        feature.addEventListener('click', function() {
            this.style.transform = 'scale(1.05)';
            setTimeout(() => {
                this.style.transform = '';
            }, 200);
        });
    });
    
    console.log(`✅ Initialized ${features.length} feature cards`);
}

function addPageLoadInfo() {
    // Show page load time
    const loadTime = Date.now() - performance.timing.navigationStart;
    
    // Create info element
    const infoDiv = document.createElement('div');
    infoDiv.style.cssText = `
        position: fixed;
        top: 10px;
        right: 10px;
        background: rgba(102, 126, 234, 0.9);
        color: white;
        padding: 10px 15px;
        border-radius: 5px;
        font-size: 12px;
        z-index: 1000;
        box-shadow: 0 2px 10px rgba(0,0,0,0.2);
    `;
    infoDiv.innerHTML = `
        📊 Page loaded in ${loadTime}ms<br>
        🌐 Static files served by C++ server
    `;
    
    document.body.appendChild(infoDiv);
    
    // Auto-hide after 5 seconds
    setTimeout(() => {
        infoDiv.style.opacity = '0';
        infoDiv.style.transition = 'opacity 0.5s';
        setTimeout(() => infoDiv.remove(), 500);
    }, 5000);
    
    console.log(`📊 Page load time: ${loadTime}ms`);
}

function addNavigationEffects() {
    // Add smooth scrolling for internal links
    const navLinks = document.querySelectorAll('nav a[href^="#"]');
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const target = document.querySelector(this.getAttribute('href'));
            if (target) {
                target.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });
    
    // Add active state to navigation
    const currentPath = window.location.pathname;
    const navLinks2 = document.querySelectorAll('nav a');
    navLinks2.forEach(link => {
        if (link.getAttribute('href') === currentPath || 
            (currentPath === '/' && link.getAttribute('href') === '/')) {
            link.style.background = '#667eea';
            link.style.color = 'white';
        }
    });
}

function showServerInfo() {
    // Display server information
    const serverInfo = {
        userAgent: navigator.userAgent,
        language: navigator.language,
        platform: navigator.platform,
        timestamp: new Date().toISOString(),
        url: window.location.href
    };
    
    console.group('🔧 Client Information');
    console.log('User Agent:', serverInfo.userAgent);
    console.log('Language:', serverInfo.language);
    console.log('Platform:', serverInfo.platform);
    console.log('Current URL:', serverInfo.url);
    console.log('Load Timestamp:', serverInfo.timestamp);
    console.groupEnd();
    
    // Test if server is responding
    testServerConnection();
}

function testServerConnection() {
    // Simple connection test
    const startTime = Date.now();
    
    fetch('/status')
        .then(response => {
            const responseTime = Date.now() - startTime;
            console.log(`✅ Server response time: ${responseTime}ms`);
            
            if (response.ok) {
                console.log('✅ Server connection: OK');
            } else {
                console.log(`⚠️ Server returned status: ${response.status}`);
            }
        })
        .catch(error => {
            const responseTime = Date.now() - startTime;
            console.log(`❌ Server connection failed after ${responseTime}ms:`, error);
        });
}

// Add some utility functions
window.serverUtils = {
    testEndpoint: function(path) {
        console.log(`🧪 Testing endpoint: ${path}`);
        fetch(path)
            .then(response => {
                console.log(`📡 ${path}: ${response.status} ${response.statusText}`);
                return response.text();
            })
            .then(data => {
                console.log(`📄 Response length: ${data.length} characters`);
            })
            .catch(error => {
                console.error(`❌ Error testing ${path}:`, error);
            });
    },
    
    showResponseHeaders: function(path) {
        fetch(path)
            .then(response => {
                console.group(`📋 Headers for ${path}`);
                for (let [key, value] of response.headers) {
                    console.log(`${key}: ${value}`);
                }
                console.groupEnd();
            })
            .catch(error => {
                console.error(`❌ Error fetching headers for ${path}:`, error);
            });
    }
};

// Add interactive demo features
function addInteractiveDemo() {
    // Create demo section if it doesn't exist
    let demoSection = document.querySelector('.demo-section');
    if (!demoSection) {
        demoSection = document.createElement('section');
        demoSection.className = 'demo-section';
        demoSection.innerHTML = `
            <h3>🧪 Interactive Demo</h3>
            <div class="demo-controls">
                <button onclick="serverUtils.testEndpoint('/')">Test Home</button>
                <button onclick="serverUtils.testEndpoint('/about.html')">Test About</button>
                <button onclick="serverUtils.testEndpoint('/nonexistent')">Test 404</button>
                <button onclick="serverUtils.showResponseHeaders('/')">Show Headers</button>
            </div>
            <div id="demo-output"></div>
        `;
        
        // Add CSS for demo section
        const style = document.createElement('style');
        style.textContent = `
            .demo-section {
                background: #fff3cd;
                padding: 20px;
                border-radius: 8px;
                margin: 20px 0;
                border-left: 4px solid #ffc107;
            }
            .demo-controls {
                display: flex;
                gap: 10px;
                flex-wrap: wrap;
                margin: 15px 0;
            }
            .demo-controls button {
                background: #667eea;
                color: white;
                border: none;
                padding: 8px 16px;
                border-radius: 4px;
                cursor: pointer;
                transition: background 0.3s;
            }
            .demo-controls button:hover {
                background: #5a67d8;
            }
            #demo-output {
                background: #f8f9fa;
                padding: 10px;
                border-radius: 4px;
                margin-top: 10px;
                font-family: monospace;
                min-height: 50px;
            }
        `;
        document.head.appendChild(style);
        
        // Insert before footer
        const footer = document.querySelector('footer');
        footer.parentNode.insertBefore(demoSection, footer);
    }
}

// Performance monitoring
function monitorPerformance() {
    if ('performance' in window) {
        window.addEventListener('load', function() {
            setTimeout(function() {
                const perfData = performance.getEntriesByType('navigation')[0];
                const metrics = {
                    'DNS Lookup': perfData.domainLookupEnd - perfData.domainLookupStart,
                    'TCP Connect': perfData.connectEnd - perfData.connectStart,
                    'Request': perfData.responseStart - perfData.requestStart,
                    'Response': perfData.responseEnd - perfData.responseStart,
                    'DOM Processing': perfData.domContentLoadedEventEnd - perfData.responseEnd,
                    'Total Load Time': perfData.loadEventEnd - perfData.navigationStart
                };
                
                console.group('⚡ Performance Metrics');
                Object.entries(metrics).forEach(([key, value]) => {
                    console.log(`${key}: ${value.toFixed(2)}ms`);
                });
                console.groupEnd();
            }, 100);
        });
    }
}

// Initialize all features
document.addEventListener('DOMContentLoaded', function() {
    addInteractiveDemo();
    monitorPerformance();
});

console.log('✅ All JavaScript features loaded successfully!');