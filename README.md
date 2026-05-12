# 🏫 Digital Twin for GI-IADS Classroom
### Industry 4.0 — Smart Educational Environment

> A comprehensive digital twin system for the GI-IADS classroom, developed as part of an Industry 4.0 initiative. This proof-of-concept demonstrates real-time monitoring, predictive analytics, and automated control in a smart educational environment.

---

## 📖 Overview

**Industry 4.0**, also known as the Fourth Industrial Revolution, represents the integration of digital technologies into manufacturing and industrial processes — encompassing AI, IoT, Big Data analytics, robotics, and automation.

A **digital twin** is a virtual replica of a physical system that maintains real-time synchronization through IoT sensors and AI-driven analytics, enabling continuous monitoring, predictive maintenance, and performance optimization of physical assets.

This project transcends simple replication by creating a dynamic, user-centric platform that delivers accurate, real-time information about classroom conditions, integrating seamlessly with existing educational workflows while providing advanced capabilities for monitoring, control, and optimization.

---

## 🏗️ System Architecture

### Overall Design Philosophy

A comprehensive full-stack IoT architecture bridges physical hardware with modern web interfaces. The system centers around a **Laravel-based API** that manages bidirectional communication between **ESP32 microcontrollers** and web clients, ensuring real-time synchronization across all system components.

```
ESP32  ──HTTP/JSON──►  Laravel API  ──WebSockets──►  Vue/React + Three.js
       ◄──HTTP/JSON──              ◄────────────────
```

The frontend uses **Vue.js** and **React** components to deliver responsive user interfaces with modern design patterns and instant state updates. The architecture separates the virtual representation (Digital Twin) from physical device control (Lamp Switcher), implementing a modular and loosely coupled system consistent with Industry 4.0 principles.

---

## ⚙️ Backend Implementation

Built on **Laravel**, the platform implements modern architectural patterns for scalable device control and user management, integrating hardware communication with web-based administration through a unified API structure.

### 🔐 Authentication System

Secured via **Laravel Sanctum** for token-based authentication, implementing three core functionalities through `AuthController`:

- **User Registration** — Validates user data via a custom `SignupRequest` class; creates records with bcrypt-hashed passwords and issues API tokens.
- **User Authentication** — Validates credentials via `Auth::attempt`; generates new API tokens on successful login.
- **Session Management** — Revokes current API tokens on logout for proper session cleanup.

```php
public function signup(SignupRequest $request)
{
    $data = $request->validated();
    $user = User::create([
        'name'     => $data['name'],
        'email'    => $data['email'],
        'password' => bcrypt($data['password']),
    ]);

    $token = $user->createToken('main')->plainTextToken;
    return response(compact('user', 'token'));
}
```

**Supported features:**
- Role-based access control (Admin / User)
- Secure signup, login, and logout flows
- JWT token validation for API requests
- Password hashing and validation

### 🛡️ Security

Security is enforced collaboratively across `AuthController.php`, `ReportController.php`, `UserController.php`, and `LampController.php`:

- `AuthController` acts as the primary gatekeeper via Laravel Sanctum — hashing passwords, authenticating users, and revoking tokens on logout.
- `ReportController`, `UserController`, and `LampController` are protected by `auth:sanctum` middleware, restricting endpoint access to token-bearing users.
- Input validation is enforced across all controllers using custom Form Request classes (e.g., `SignupRequest`, `StoreUserRequest`) to sanitize data and prevent injection attacks.
- `LampController` delegates logic to a `LampService`, providing an additional abstraction layer for security checks.

**Industry 4.0 Compliance measures:**

| Feature | Implementation |
|---|---|
| Password Policy | Minimum 8 characters with letters/symbols; bcrypt hashing |
| Token Management | Short-lived tokens (configurable); revocation on logout |
| Middleware Protection | `auth:sanctum` guards all API routes |
| CSRF Protection | `VerifyCsrfToken` middleware |
| Audit Trails | All login attempts logged (success/failure) |
| Scalability | Sanctum tokens support microservices architecture |
| Device Management | User model links to IoT devices |

### 🔄 CRUD Operations

Full Create, Read, Update, Delete workflows with security-first design:

```php
public function store(StoreUserRequest $request) {
    $data = $request->validated();
    $data['password'] = bcrypt($data['password']); // Hash before storage
    $user = User::create($data);                   // Mass assignment protection
    return new UserResource($user);                // Secure serialization
}
```

**Key controllers:**
- `AuthController` — Signup, login, and logout logic
- `Authenticate` Middleware — Redirects unauthenticated users
- `RedirectIfAuthenticated` — Prevents authenticated users from accessing auth pages
- `UpdateUserRequest` — Validates user profile updates
- `User` Model — Mass-assignable fields, hidden password attribute, data casting

### 🏛️ Technical Architecture

- RESTful API with clear separation between authentication, device control, and user management
- Laravel Sanctum for secure token-based authentication across all endpoints
- Service layer architecture with controllers delegating to specialized services
- Eloquent ORM for efficient database operations
- Form Request classes for input validation
- Built-in pagination and query filtering for optimized data retrieval

---

## 📟 Device Management System

### Device Dashboard

A comprehensive management interface for handling devices (computers, lamps, sensors) through full CRUD operations. Each device maintains:

- **Status** — Device condition: `OK`, `Error`, `Maintenance Required`
- **State** — Operational mode: `on/off` for lamps, `active/idle` for computers
- **Location Reference** — Table or position assignment for filtering and monitoring

Computers are assigned to specific tables, which serve as references for filtering, visualization, and monitoring errors and reports.

### Report Management API

`ReportController` handles comprehensive report creation and status management:

```php
class ReportController extends Controller
{
    public function store(Request $request)
    {
        $data = $request->validate([
            'device_id'   => 'required|integer',
            'device_type' => 'required|string',
            'text'        => 'required|string',
        ]);
        $report = Report::create($data);
        return response()->json($report, 201);
    }

    public function update(Request $request, Report $report)
    {
        $report->update($request->only('fixed'));
        return response()->json($report);
    }
}
```

- `store()` — Validates and saves new reports with device ID, type, and description
- `update()` — Updates the `fixed` status of reports for issue tracking

---

## ✨ Core Functionalities

| Feature | Description |
|---|---|
| **User Management** | Role-based account creation for students and instructors with access controls |
| **Activity Logging** | Comprehensive audit trails recording all system interactions with timestamps |
| **Incident Reporting** | Integrated alarm system for reporting malfunctions and issues |
| **Analytics & Reporting** | Insights into classroom usage patterns and system performance |
| **IoT Integration** | Extensible API framework for seamless integration of additional IoT devices |
| **Device Control** | Real-time control and monitoring of connected devices with state logging |
| **Real-time Dashboard** | Live system status, device controls, and performance metrics |

---

## 🖥️ User Interface Design

### 2D Digital Twin Visualization

The core of the system is a 2D visual representation of the GI-IADS classroom, serving as the primary interface for users to interact with the physical space.

**Features:**
- **Spatial Mapping** — Floor plan showing precise locations of desks, equipment, lighting fixtures, and classroom elements
- **Interactive Device Controls** — Direct click interaction with visual device representations
- **Real-time Status Updates** — Visual elements change color, brightness, or appearance to reflect current physical device states
- **Contextual Information Display** — Hover or selection reveals detailed info panels with device specs, readings, and historical data
- **Visual State Indicators** — Color-coded status: 🟢 Active/Normal · 🔴 Malfunction/Offline · 🟡 Maintenance Required

### Dashboard

The dashboard provides comprehensive oversight of all connected IoT devices and system activities:

- Live device status indicators with intuitive color-coded states
- Comprehensive user activity monitoring and session management
- Quick-access control buttons for immediate device operations
- Real-time synchronization ensuring instant updates across all connected clients
- Role-based access controls for students and instructors

### Database Design

| Table | Description |
|---|---|
| `users` | User credentials, role assignments, and authentication tokens |
| `devices` | Current device states, configurations, and operational parameters |
| `reports` | Device issue reporting with `device_id`, `device_type`, description text, and `fixed` status |
| `activity_logs` | System interactions with precise timestamps for audit and analytics |

The logging system captures every user interaction and device state change, enabling complete audit trails and detailed analytics on classroom usage patterns.

---

## 💡 IoT Implementation: Light Control System

### Hardware Integration

The **ESP8266** microcontroller serves as the primary physical interface between the digital twin platform and the classroom environment. This WiFi-enabled microcontroller provides robust IoT connectivity and real-time hardware control.

The hardware configuration includes:
- GPIO pin management for LED control
- WiFi connectivity protocols
- HTTP client functionality for API communication

The ESP8266 operates on an efficient **polling architecture**, continuously monitoring API endpoints for state changes and executing corresponding hardware adjustments.

### Lamp Switcher — React Component

```javascript
function LampSwitcher({ lampId }) {
  const [isOn, setIsOn]       = useState(false);
  const [loading, setLoading] = useState(true);
  const [error, setError]     = useState(false);

  // Component logic handles state management and API communication
}
```

### Integration & Communication

The implementation uses **Axios** for HTTP requests between frontend and backend:

- `GET /api/leds/{lampId}` — Fetch current lamp state
- `POST /api/leds/{lampId}/toggle` — Toggle lamp state

**Laravel Backend:**
- `LampController` delegates control logic to `LampService`
- Controller methods act as endpoints forwarding requests to the Lamp Switcher
- Abstraction layer separating frontend from direct device interaction

### Communication Protocol

The ESP8266–Laravel API communication follows a standardized **JSON-based protocol**:

1. Microcontroller performs periodic `GET` requests to retrieve current device states
2. Compares received states with local state variables to detect discrepancies
3. On change detection, immediately synchronizes physical device states with the digital twin

The database maintains a **single authoritative record** for all device states, ensuring consistency across all system interactions.

---

## 📊 Testing & Performance

### System Performance Metrics

- ⚡ API response times consistently **below 300ms** for device control operations
- 📡 Dashboard maintains real-time updates without perceptible delays
- ✅ Validated ability to handle concurrent user sessions and multiple device interactions
- 🔗 ESP8266 maintains stable connectivity and responds correctly to web interface commands

### User Experience Evaluation

Initial testing with students yielded positive feedback on usability and functionality. Users consistently rated the interface as intuitive and appreciated real-time device control. The role-based access system effectively provides appropriate functionality levels for different user types while maintaining security.

---

## 🔭 Future Development & Scalability

The modular architecture provides a robust foundation for future enhancements:

- [ ] Environmental monitoring sensors (temperature, humidity, air quality)
- [ ] Advanced analytics and machine learning capabilities
- [ ] Integration with existing educational management systems
- [ ] Mobile application development for enhanced accessibility
- [ ] Expansion to additional classrooms and facilities



## 📝 Conclusion

This project successfully demonstrates the practical implementation of Industry 4.0 principles in educational environments through a comprehensive digital twin system. The integration of IoT hardware with modern web technologies creates a solid foundation for enhanced classroom management.
