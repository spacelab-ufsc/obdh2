% Compact PDF layout for the wide packet summary. The equivalent RST table
% remains the source for HTML and for Sphinx's cross-reference bookkeeping.
\begin{table}[ht]
    \centering
    \small
    \begin{tabular}{L{1.8cm}lcllcc}
        \toprule[1.5pt]
        \multirow{2}{*}{\textbf{Link}} & \multirow{2}{*}{\textbf{Packet Name}} & \multicolumn{4}{c}{\textbf{Payload}} & \multirow{2}{*}{\textbf{Access}} \\
        \cmidrule{3-6}
                                 &                     & \textbf{ID} & \textbf{Source Callsign} & \textbf{Data (up to 212 bytes)} & \textbf{Size (bytes)} & \\
        \midrule
        Downlink                 & EPS data            & 00h & \multirow{3}{*}{`` '' + ``PY0EFS''} & EPS data                                          & 46        & Public \\
        (VHF)                    & Message broadcast   & 01h &                                     & Requester + dst. callsign + message               & 22 to 60  & Public \\
                                 & Ping answer         & 02h &                                     & Requester callsign                                & 15        & Public \\
        \midrule
                                 & General telemetry   & 10h & \multirow{6}{*}{`` '' + ``PY0EFS''} & OBDH/EPS data                                     & 78        & Public \\
                                 & Data request answer & 11h &                                     & Requester callsign + data ID + ts. + data         & 20 to 220 & Public \\
        Downlink                 & Payload data        & 12h &                                     & Payload ID + payload data                         & 9 to 220  & Public \\
        (UHF)                    & TC feedback         & 13h &                                     & Req. callsign + TC packet ID + timestamp          & 20        & Public \\
                                 & Parameter value     & 14h &                                     & Req. callsign + Sub. ID + Param. ID + Param. Val. & 21        & Public \\
                                 & Packet broadcast    & 15h &                                     & Data of ``Transmit packet'' TC                    & 8 to 60   & Public \\
        \midrule
        \multirow{16}{*}{Uplink} & Ping request        & 40h & \multirow{16}{*}{Any Callsign}      & None                                              & 8         & Public  \\
                                 & Data request        & 41h &                                     & Data ID + Start ts. + End ts. + Hash              & 37        & Private \\
                                 & Broadcast Message   & 42h &                                     & Dst. callsign + message                           & 15 to 53  & Public  \\
                                 & Enter hibernation   & 43h &                                     & Hibernation in hours + Hash                       & 30        & Private \\
                                 & Leave hibernation   & 44h &                                     & Hash                                              & 28        & Private \\
                                 & Activate module     & 45h &                                     & Module ID + Hash                                  & 29        & Private \\
                                 & Deactivate module   & 46h &                                     & Module ID + Hash                                  & 29        & Private \\
                                 & Activate payload    & 47h &                                     & Payload ID + Hash                                 & 29        & Private \\
                                 & Deactivate payload  & 48h &                                     & Payload ID + Hash                                 & 29        & Private \\
                                 & Erase memory        & 49h &                                     & Memory ID + Hash                                  & 29        & Private \\
                                 & Force reset         & 4Ah &                                     & Hash                                              & 28        & Private \\
                                 & Get payload data    & 4Bh &                                     & Payload ID + Args. + Hash                         & 41        & Private \\
                                 & Set parameter       & 4Ch &                                     & Subsystem ID + Param. ID + Param. value + Hash    & 34        & Private \\
                                 & Get parameter       & 4Dh &                                     & Subsystem ID + Parameter ID + Hash                & 30        & Private \\
                                 & Transmit packet     & 4Eh &                                     & Req. callsign + Any sequence of bytes + Hash      & 29 to 73  & Private \\
                                 & Update TLE          & 4Fh &                                     & Binary TLE line + Hash                            & 78        & Private \\
        \bottomrule[1.5pt]
    \end{tabular}
    \caption{Telecommunication packets and their content.}
    \label{firmware:tab-packets-struct}
\end{table}
